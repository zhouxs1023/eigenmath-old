//	George Weigt
//	gweigt@yahoo.com
//
//	This file does all the things required by the OS X API.
//
//	The starting point for a symbolic computation is in run.cpp
//
//	Input is scanned in scan.cpp
//
//	Expression evaluation is done in eval.cpp
//
//	Output is formatted in cmdisplay.cpp
//
//	The window display code is in window.cpp

#include <Carbon/Carbon.h>

#include "YASTControl.h"

#define SMALL_FONT 1
#define DEFAULT_FONT 2
#define TIMES_FONT 3
#define ITALIC_TIMES_FONT 4
#define SYMBOL_FONT 5
#define ITALIC_SYMBOL_FONT 6
#define SMALL_TIMES_FONT 7
#define SMALL_ITALIC_TIMES_FONT 8
#define SMALL_SYMBOL_FONT 9 
#define SMALL_ITALIC_SYMBOL_FONT 10

#if 1
#define DEFAULT_WIDTH 680
#define DEFAULT_HEIGHT 480
#else
#define DEFAULT_WIDTH 400
#define DEFAULT_HEIGHT 240
#endif

int client_width = DEFAULT_WIDTH;
int client_height = DEFAULT_HEIGHT;
int line_height = 20;
int left_margin = 5;
int right_margin = 5;
int display_x;
int display_y;
int display_width;
int display_height;
int max_x;
int max_y;
int total_w;
int total_h;
int scroll_bar_dim = 15;
int grow_dim = 0; // set to 15 to remove overlap w/grow control
int input_control_height = 28;
int update_display_request;

extern int esc_flag;
static int running;
static unsigned int timer;

struct {
    int ascent, descent, width;
} text_metric[11];

char *button_cmd[10] = {
    "d",
    "integral",
    "condense",
    "expand",
    "rationalize",
    "simplify",
    "factor",
    "roots",
    "draw",
    "float",
};

char *button_name[12] = {
    "Derivative",
    "Integral",
    "Condense",
    "Expand",
    "Rationalize",
    "Simplify",
    "Factor",
    "Roots",
    "Draw",
    "Float",
    "Edit Script",
    "Run Script",
};

#define YYFONT 0
#define YYSIZE 13

pascal OSStatus MainWindowCommandHandler( EventHandlerCallRef handlerRef, EventRef event, void *userData );

extern void do_up_arrow(void);
extern void do_down_arrow(void);
extern void printstr(char *);
extern void clear(void);
extern void draw_display(void);
extern void run(char *);
extern void do_help(int);
extern char *get_tty_buf(void);

void do_control_stuff(void);
void create_controls(void);
void create_input_control(void);
void create_edit_control(void);
void create_graph_control(void);
void create_enter_button(void);
void create_run_button(void);
void create_graph_window(void);
void create_main_window(void);
void update_curr_cmd(char *);

static void file_svas(void);
static void do_save(void);
static void file_open(void);
static void do_example(int);
static void select_edit_window(void);
static void update_edit_control(void);
static void select_font(int);
static void update_scroll_bars(void);
static void vscroll_f(ControlRef, ControlPartCode);
static void hscroll_f(ControlRef, ControlPartCode);
static void do_main_help(int);
static void do_return_key(void);
static void do_button(char *);
static void do_run_button(void);
static void draw_display_now(void);
static void send_user_event(void);
static void process_user_event(void);
static void get_script(void);
static void do_resize(void);
static void copy_display(void);
static void copy_tty(void);
static void update_display(void);

WindowRef gwindow;
WindowRef edit_window;

ControlHandle inputcontrol;
ControlHandle edit_control;
ControlHandle hscroll;
ControlHandle vscroll;
ControlHandle buttons[20];

char filename[1000], path[1000];

#define MAX_PROGRAM_SIZE 100000
char program_buf[MAX_PROGRAM_SIZE + 1];

/* Register help book */

static void
register_help(void)
{
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    if (mainBundle)
    {
        CFURLRef bundleURL = NULL;
        CFRetain(mainBundle);
        bundleURL = CFBundleCopyBundleURL(mainBundle);
        if (bundleURL)
        {
            FSRef bundleFSRef;
            if (CFURLGetFSRef(bundleURL, &bundleFSRef)) {
                FSRefMakePath(&bundleFSRef, (UInt8 *) path, 1000);
                AHRegisterHelpBook(&bundleFSRef);
            }
            CFRelease(bundleURL);
        }
        CFRelease(mainBundle);
    }
}

static void
timer_function(EventLoopTimerRef ref, void *data)
{
    if (running)
        send_user_event();
}

#define NEVENT 6

EventTypeSpec commSpec[NEVENT] = {
    {kEventClassCommand, kEventProcessCommand},
    {kEventClassKeyboard, kEventRawKeyDown},
    {kEventClassKeyboard, 1234},
    {kEventClassWindow, kEventWindowResizeCompleted},
    {kEventClassWindow, kEventWindowZoomed},
    {kEventClassWindow, kEventWindowClosed},
};

int main(int argc, char* argv[])
{
    IBNibRef 		nibRef;
    OSStatus err;

    err = TXNInitTextension(NULL, 0, 0);
    //require_noerr( err, CantGetNibRef );

    // Create a Nib reference passing the name of the nib file (without the .nib extension)
    // CreateNibReference only searches into the application bundle.
    err = CreateNibReference(CFSTR("main"), &nibRef);
    require_noerr( err, CantGetNibRef );
    
    // Once the nib reference is created, set the menu bar. "MainMenu" is the name of the menu bar
    // object. This name is set in InterfaceBuilder when the nib is created.
    err = SetMenuBarFromNib(nibRef, CFSTR("MenuBar"));
    require_noerr( err, CantSetMenuBar );
    
    // Then create a window. "MainWindow" is the name of the window object. This name is set in 
    // InterfaceBuilder when the nib is created.
//    err = CreateWindowFromNib(nibRef, CFSTR("MainWindow"), &window);
 //   require_noerr( err, CantCreateWindow );


    // We don't need the nib reference anymore.
    DisposeNibReference(nibRef);

    register_help();

    // The window was created hidden so show it.
    //ShowWindow( window );

    create_main_window();

    //SetKeyboardFocus(gwindow, inputcontrol, kControlFocusNextPart);

    InstallWindowEventHandler(
        gwindow,
        NewEventHandlerUPP(MainWindowCommandHandler),
        NEVENT,
        commSpec,
        NULL,
        NULL);

    InstallEventLoopTimer(
        GetMainEventLoop(),
        1, // 1 second
        1, // 1 second
        NewEventLoopTimerUPP(timer_function),
        NULL,
        NULL);

    RunApplicationEventLoop();

CantSetMenuBar:
CantGetNibRef:
	return 0;
}

OSStatus kopencmd(void);

#define FILE_MENU_ID 2

#define FILE_NEW_ITEM 1
#define FILE_OPEN_ITEM 2

#define FILE_CLOSE_ITEM 4
#define FILE_SAVE_ITEM 5
#define FILE_SAVEAS_ITEM 6

#define EXAMPLES_MENU_ID 4
#define EXAMPLES_VECTOR_CALCULUS_ITEM 1

#if 0

static void
update_file_menu(void)
{
    MenuRef m;
    m = GetMenuHandle(FILE_MENU_ID);
    if (script_window == NULL) {
        EnableMenuItem(m, FILE_NEW_ITEM);
        EnableMenuItem(m, FILE_OPEN_ITEM);
        DisableMenuItem(m, FILE_CLOSE_ITEM);
        DisableMenuItem(m, FILE_SAVE_ITEM);
        DisableMenuItem(m, FILE_SAVEAS_ITEM);
    } else {
        DisableMenuItem(m, FILE_NEW_ITEM);
        DisableMenuItem(m, FILE_OPEN_ITEM);
        EnableMenuItem(m, FILE_CLOSE_ITEM);
        EnableMenuItem(m, FILE_SAVE_ITEM);
        EnableMenuItem(m, FILE_SAVEAS_ITEM);
    }
}

#endif

pascal OSStatus
MainWindowCommandHandler(EventHandlerCallRef handlerRef, EventRef event, void *userData)
{
    OSStatus err = noErr;
    HICommand command;
    int yclass, kind;

    yclass = GetEventClass(event);
    kind = GetEventKind(event);
    
    //printf("class %c%c%c%c kind %d\n", class >> 24 & 0xff, class >> 16 & 0xff, class >> 8 & 0xff, class & 0xff, kind);
    
    if (yclass == kEventClassKeyboard && kind == 1234) {
        process_user_event();
        return noErr;
    }

    if (yclass == kEventClassWindow) {
        switch (kind) {
        case kEventWindowResizeCompleted:
        case kEventWindowZoomed:
            do_resize();
            break;
        case kEventWindowClosed:
            exit(0);
            break;
        default:
            err = eventNotHandledErr;
            break;
        }
        return err;
    }

    if (yclass == kEventClassKeyboard) {
        char keycode;
        GetEventParameter(event, kEventParamKeyMacCharCodes, typeChar, NULL, sizeof (char), NULL, &keycode);
        if (keycode == 27) {
            esc_flag = 1;
            return noErr;
        } else if (running)
            return eventNotHandledErr;
        else if (keycode == 13) {
            do_return_key();
            return noErr;
        } else if (keycode == 30) {
            do_up_arrow();
            return noErr;
        } else if (keycode == 31) {
            do_down_arrow();
            return noErr;
        } else
            return eventNotHandledErr;
    }

//    y = GetEventKind(event);

//printf("%c%c%c%c %08x\n", (x >> 24), (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff, y);

    GetEventParameter( event, kEventParamDirectObject, typeHICommand, NULL, sizeof(HICommand), NULL, &command );

//printf("%08x, %08x\n", command.attributes, command.commandID);

    if ((command.commandID & 0xffff0000) == 0xcafe0000) {
        do_button(button_cmd[command.commandID & 0xff]);
        return noErr;
    }

    switch( command.commandID ) {

    case 'PROG':
        if (running == 0)
            select_edit_window();
        break;

    case 'RUN ':
        if (running == 0)
            do_run_button();
        break;

    // menu events

    case 'abou':
        if (running == 0) {
            printstr("This is Eigenmath version 102.\n");
            update_display();
        }
        break;

    case 'new ':
        if (running == 0) {
            select_edit_window();
            *filename = 0;
            *program_buf = 0;
            update_edit_control();
        }
        break;

    case 'open':
        if (running == 0)
            file_open();
        break;

    case 'clos':
        break;

    case 'save':
        if (*filename == 0)
            file_svas();
        else
            do_save();
        break;

    case 'svas':
        file_svas();
        break;

// edit menu

    case 'CPY1':
        copy_display();
        break;
    case 'CPY2':
        copy_tty();
        break;

// howto menu

    case 'H001':
        do_main_help(1);
        break;
    case 'H002':
        do_main_help(2);
        break;
    case 'H003':
        do_main_help(3);
        break;
    case 'H004':
        do_main_help(4);
        break;
    case 'H005':
        do_main_help(5);
        break;
    case 'H006':
        do_main_help(6);
        break;
    case 'H007':
        do_main_help(7);
        break;
    case 'H008':
        do_main_help(8);
        break;
    case 'H009':
        do_main_help(9);
        break;
    case 'H010':
        do_main_help(10);
        break;
    case 'H011':
        do_main_help(11);
        break;
    case 'H012':
        do_main_help(12);
        break;
    case 'H013':
        do_main_help(13);
        break;

    // sample scripts

    case 'GMA ':
        do_example(0);
        break;
    case 'VC  ':
        do_example(1);
        break;
    case 'RM  ':
        do_example(2);
        break;
    case 'QHO ':
        do_example(3);
        break;
    case 'HW  ':
        do_example(4);
        break;
    case 'SSM ':
        do_example(5);
        break;
    case 'FPDE':
        do_example(6);
        break;

    default:
        err = eventNotHandledErr;
        break;
    }
    return err;
}

// returns a malloc value

char *
get_curr_cmd(void)
{
    int i;
    Size len;
    char *s;
    GetControlDataSize(inputcontrol, 0, kControlEditTextTextTag, &len);
    s = (char *) malloc(len + 1);
    if (s == NULL)
        exit(1);
    GetControlData(inputcontrol, 0, kControlEditTextTextTag, len, s, &len);
    s[len] = 0;
    // trim trailing spaces
    for (i = len - 1; i >= 0; i--) {
        if (isspace(s[i]))
            s[i] = 0;
        else
            break;
    }
    return s;
}

void
update_curr_cmd(char *s)
{
    //DeactivateControl(inputcontrol);
    SetControlData(inputcontrol, 0, kControlEditTextTextTag, strlen(s), s);
    //ActivateControl(inputcontrol);
    DrawOneControl(inputcontrol);
}

void
select_all_input(int n)
{
    ControlEditTextSelectionRec sel;
    sel.selStart = 0;
    sel.selEnd = n;
    DeactivateControl(inputcontrol); // this is needed otherwise hilite is wrong
    SetControlData(inputcontrol, 0, kControlEditTextSelectionTag, sizeof sel, &sel);
    ActivateControl(inputcontrol);
    DrawOneControl(inputcontrol);
}

static void
select_edit_window(void)
{
    Rect r;

    if (edit_window) {
        SelectWindow(edit_window);
        return;
    }

    r.left = 80;
    r.top = 44;
    r.right = r.left + DEFAULT_WIDTH + grow_dim;
    r.bottom = r.top + DEFAULT_HEIGHT + grow_dim;

    CreateNewWindow(6, kWindowCollapseBoxAttribute | kWindowStandardHandlerAttribute, &r, &edit_window);

    // script edit control

    r.left = 0;
    r.top = 0;
    r.right = DEFAULT_WIDTH + grow_dim;
    r.bottom = DEFAULT_HEIGHT + grow_dim;

    CreateYASTControl(edit_window, &r, &edit_control);

    // This seems to fix the runt cursor problem

    strcpy(program_buf, "Hello, world\n");
    update_edit_control();
    *program_buf = 0;
    update_edit_control();

    ShowWindow(edit_window);

    SetKeyboardFocus(edit_window, edit_control, kControlFocusNextPart);

    InstallWindowEventHandler(edit_window, NewEventHandlerUPP(MainWindowCommandHandler), 1, commSpec, (void *) edit_window, NULL);
}

#define BLUE_SHIM 6

void
create_input_control(void)
{
    Rect r;

    r.left = 0;
    r.top = display_height + scroll_bar_dim;
    r.right = client_width;
    r.bottom = r.top + input_control_height;

    r.left += BLUE_SHIM;
    r.top += BLUE_SHIM;
    r.right -= BLUE_SHIM;
    r.bottom -= BLUE_SHIM - 1;

    CreateEditTextControl(gwindow, &r, NULL, FALSE, FALSE, NULL, &inputcontrol);
}

#define SHIM 2

#define AA ((2 * line_height - 3 * SHIM) / 2)

void
create_buttons(void)
{
    int i, j, k;
    char *s;
    Rect r;
    CFStringRef str;

    for (i = 0; i < 6; i++) {
        j = SHIM + i * (client_width - SHIM) / 6;
        k = (i + 1) * (client_width - SHIM) / 6;
        r.left = j;
        r.top = client_height - 2 * line_height + SHIM;
        r.right = k;
        r.bottom = r.top + AA;
        s = button_name[2 * i];
        str = CFStringCreateWithCString(NULL, s, kCFStringEncodingMacRoman);
        CreateBevelButtonControl(gwindow, &r, str, 0, 0, 0, 0, 0, 0, buttons + 2 * i);
        CFRelease(str);
        r.left = j;
        r.top = client_height - AA - SHIM;
        r.right = k;
        r.bottom = r.top + AA;
        s = button_name[2 * i + 1];
        str = CFStringCreateWithCString(NULL, s, kCFStringEncodingMacRoman);
        CreateBevelButtonControl(gwindow, &r, str, 0, 0, 0, 0, 0, 0, buttons + 2 * i + 1);
        CFRelease(str);
    }

    for (i = 0; i < 10; i++)
        SetControlCommandID(buttons[i], 0xcafe0000 + i);

    SetControlCommandID(buttons[10], 'PROG');
    SetControlCommandID(buttons[11], 'RUN ');
}

extern OSStatus CreateYASTControl(WindowRef, Rect *, ControlRef *);

static void
do_font_metric(void)
{
    FontInfo font;

    select_font(SMALL_FONT);
    GetFontInfo(&font);
    text_metric[SMALL_FONT].ascent = font.ascent;
    text_metric[SMALL_FONT].descent = font.descent;
    text_metric[SMALL_FONT].width = 2 * CharWidth(' ');

    select_font(DEFAULT_FONT);
    GetFontInfo(&font);
    text_metric[DEFAULT_FONT].ascent = font.ascent;
    text_metric[DEFAULT_FONT].descent = font.descent;
    text_metric[DEFAULT_FONT].width = 2 * CharWidth(' ');

    select_font(TIMES_FONT);
    GetFontInfo(&font);
    text_metric[TIMES_FONT].ascent = font.ascent;
    text_metric[TIMES_FONT].descent = font.descent;
    text_metric[TIMES_FONT].width = 2 * CharWidth(' ');

    select_font(ITALIC_TIMES_FONT);
    GetFontInfo(&font);
    text_metric[ITALIC_TIMES_FONT].ascent = font.ascent;
    text_metric[ITALIC_TIMES_FONT].descent = font.descent;
    text_metric[ITALIC_TIMES_FONT].width = 2 * CharWidth(' ');

    select_font(SYMBOL_FONT);
    GetFontInfo(&font);
    text_metric[SYMBOL_FONT].ascent = font.ascent;
    text_metric[SYMBOL_FONT].descent = font.descent;
    text_metric[SYMBOL_FONT].width = 2 * CharWidth(' ');

    select_font(ITALIC_SYMBOL_FONT);
    GetFontInfo(&font);
    text_metric[ITALIC_SYMBOL_FONT].ascent = font.ascent;
    text_metric[ITALIC_SYMBOL_FONT].descent = font.descent;
    text_metric[ITALIC_SYMBOL_FONT].width = 2 * CharWidth(' ');

    select_font(SMALL_TIMES_FONT);
    GetFontInfo(&font);
    text_metric[SMALL_TIMES_FONT].ascent = font.ascent;
    text_metric[SMALL_TIMES_FONT].descent = font.descent;
    text_metric[SMALL_TIMES_FONT].width = 2 * CharWidth(' ');

    select_font(SMALL_ITALIC_TIMES_FONT);
    GetFontInfo(&font);
    text_metric[SMALL_ITALIC_TIMES_FONT].ascent = font.ascent;
    text_metric[SMALL_ITALIC_TIMES_FONT].descent = font.descent;
    text_metric[SMALL_ITALIC_TIMES_FONT].width = 2 * CharWidth(' ');

    select_font(SMALL_SYMBOL_FONT);
    GetFontInfo(&font);
    text_metric[SMALL_SYMBOL_FONT].ascent = font.ascent;
    text_metric[SMALL_SYMBOL_FONT].descent = font.descent;
    text_metric[SMALL_SYMBOL_FONT].width = 2 * CharWidth(' ');

    select_font(SMALL_ITALIC_SYMBOL_FONT);
    GetFontInfo(&font);
    text_metric[SMALL_ITALIC_SYMBOL_FONT].ascent = font.ascent;
    text_metric[SMALL_ITALIC_SYMBOL_FONT].descent = font.descent;
    text_metric[SMALL_ITALIC_SYMBOL_FONT].width = 2 * CharWidth(' ');
}

static void
create_scroll_bar_controls(void)
{
    Rect r;

    // horizontal scroll bar

    r.left = 0;
    r.top = display_height;
    r.right = display_width;
    r.bottom = r.top + scroll_bar_dim;

    CreateScrollBarControl(gwindow, &r, display_x, 0, max_x, display_width, TRUE, hscroll_f, &hscroll);

    // vertical scroll bar

    r.left = display_width;
    r.top = 0;
    r.right = r.left + scroll_bar_dim;
    r.bottom = display_height;

    CreateScrollBarControl(gwindow, &r, display_y, 0, max_y, display_height, TRUE, vscroll_f, &vscroll);
}

//#define WINATTR (kWindowFullZoomAttribute | kWindowCollapseBoxAttribute | kWindowResizableAttribute | kWindowStandardHandlerAttribute)

#define WINATTR (kWindowStandardDocumentAttributes | kWindowStandardHandlerAttribute)

void
create_main_window(void)
{
    Rect r;

    r.left = 0;
    r.top = 44;
    r.right = r.left + client_width + grow_dim;
    r.bottom = r.top + client_height + grow_dim;

    //CreateNewWindow(6, kWindowCollapseBoxAttribute | kWindowStandardHandlerAttribute, &r, &gwindow);

    CreateNewWindow(6, WINATTR, &r, &gwindow);

    //SetWindowTitleWithCFString(gwindow, CFSTR("Eigenmath 67"));

    display_width = client_width - scroll_bar_dim;
    display_height = client_height - 2 * line_height - input_control_height - scroll_bar_dim;

    do_font_metric();

    create_scroll_bar_controls();
    create_input_control();
    create_buttons();

    ShowWindow(gwindow);

    SetKeyboardFocus(gwindow, inputcontrol, kControlFocusNextPart);
}

#define DISPLAY_SIZE 24

#define DISPLAY_SCRIPT_SIZE 18

static void
select_font(int font)
{
    switch (font) {
    case SMALL_FONT: // for graph axis labels
        TextFont(22);
        TextSize(12);
        TextFace(0);
        break;
    case DEFAULT_FONT: // for echoing user input
        TextFont(YYFONT);
        TextSize(YYSIZE);
        TextFace(0);
        break;
    case TIMES_FONT:
        TextFont(20);
        TextSize(DISPLAY_SIZE);
        TextFace(0);
        break;
    case ITALIC_TIMES_FONT:
        TextFont(20);
        TextSize(DISPLAY_SIZE);
        TextFace(italic);
        break;
    case SYMBOL_FONT:
        TextFont(23);
        TextSize(DISPLAY_SIZE);
        TextFace(0);
        break;
    case ITALIC_SYMBOL_FONT:
        TextFont(23);
        TextSize(DISPLAY_SIZE);
        TextFace(italic);
        break;
    case SMALL_TIMES_FONT:
        TextFont(20);
        TextSize(DISPLAY_SCRIPT_SIZE);
        TextFace(0);
        break;
    case SMALL_ITALIC_TIMES_FONT:
        TextFont(20);
        TextSize(DISPLAY_SCRIPT_SIZE);
        TextFace(italic);
        break;
    case SMALL_SYMBOL_FONT:
        TextFont(23);
        TextSize(DISPLAY_SCRIPT_SIZE);
        TextFace(0);
        break;
    case SMALL_ITALIC_SYMBOL_FONT:
        TextFont(23);
        TextSize(DISPLAY_SCRIPT_SIZE);
        TextFace(italic);
        break;
    }
}

static void
update_edit_control(void)
{
    int i, n;
    YASTControlEditTextSelectionRec sel;
    n = strlen(program_buf);
    for (i = 0; i < n; i++)
        if (program_buf[i] == '\n')
            program_buf[i] = '\r';
    HideControl(edit_control);
    SetControlData(edit_control, 0, kControlEditTextTextTag, n, program_buf);
    sel.selStart = 0;
    sel.selEnd = 0;
    SetControlData(edit_control, 0, kYASTControlSelectionRangeTag, sizeof sel, &sel);
    ShowControl(edit_control);
}

extern char *example_script[7];

static void
do_example(int n)
{
    if (running)
        return;
    *filename = 0;
    select_edit_window(); // may clobber program_buf
    strcpy(program_buf, example_script[n]);
    update_edit_control();
}

static void
do_open(void)
{
    int n;
    FILE *f;
    select_edit_window(); // may clobber program_buf
    f = fopen(filename, "r");
    if (f == NULL) {
        *filename = 0;
        *program_buf = 0;
    } else {
        n = fread(program_buf, 1, MAX_PROGRAM_SIZE, f);
        fclose(f);
        program_buf[n] = 0;
        fclose(f);
    }
    update_edit_control();
}

static void
do_save(void)
{
    FILE *f;
    get_script();
    f = fopen(filename, "w");
    if (f == NULL)
        return;
    fwrite(program_buf, 1, strlen(program_buf), f);
    fclose(f);
}

// "save as" dialog gives us directory path and file name separately

static pascal void
file_svas_callback(NavEventCallbackMessage msg, NavCBRecPtr p, void *o)
{
    int n;
    NavReplyRecord r;
    AEDesc d;
    FSRef f;
    if (msg != kNavCBUserAction)
        return;
    if (NavDialogGetUserAction(p->context) != kNavUserActionSaveAs)
        return;
    NavDialogGetReply(p->context, &r);
    AECoerceDesc(&r.selection, typeFSRef, &d);
    AEGetDescData(&d, (void *) &f, sizeof (FSRef));
    FSRefMakePath(&f, (UInt8 *) filename, 1000);
    n = strlen(filename);
    if (n && filename[n - 1] != '/')
        filename[n++] = '/';
    CFStringGetCString(r.saveFileName, filename + n, 1000 - n, 0);
    do_save();
    AEDisposeDesc(&d);
    NavDisposeReply(&r);
}

static void
file_svas(void)
{
    char *s, *t;
    NavEventUPP upp;
    NavDialogRef dialog;
    NavDialogCreationOptions opt;

    upp = NewNavEventUPP(file_svas_callback);
    NavGetDefaultDialogCreationOptions(&opt);

    // isolate file name

    if (*filename) {
        s = filename;
        t = filename;
        while (*t) {
            if (*t == '/')
                s = t + 1;
            t++;
        }
        opt.saveFileName = CFStringCreateWithCString(NULL, s, 0);
    }

    NavCreatePutFileDialog(&opt, 0, 0, upp, NULL, &dialog);
    NavDialogRun(dialog);
    NavDialogDispose(dialog);
    DisposeNavEventUPP(upp);
}

static pascal void
file_open_callback(NavEventCallbackMessage msg, NavCBRecPtr p, void *o)
{
    NavReplyRecord r;
    AEDesc d;
    FSRef f;
    if (msg != kNavCBUserAction)
        return;
    if (NavDialogGetUserAction(p->context) != kNavUserActionOpen)
        return;
    NavDialogGetReply(p->context, &r);
    AECoerceDesc(&r.selection, typeFSRef, &d);
    AEGetDescData(&d, (void *) &f, sizeof (FSRef));
    FSRefMakePath(&f, (UInt8 *) filename, 1000);
    do_open();
    AEDisposeDesc(&d);
    NavDisposeReply(&r);
}

static void
file_open(void)
{
    NavEventUPP upp;
    NavDialogRef dialog;
    NavDialogCreationOptions opt;
    upp = NewNavEventUPP(file_open_callback);
    NavGetDefaultDialogCreationOptions(&opt);
    NavCreateGetFileDialog(&opt, NULL, upp, NULL, NULL, NULL, &dialog);
    NavDialogRun(dialog);
    NavDialogDispose(dialog);
    DisposeNavEventUPP(upp);
}

// mac draw func args are 16 bits so must clip

#define CLIP 32000

void
draw_text(int font, int x, int y, char *s, int len)
{
    if (x < -CLIP || x > CLIP || y < -CLIP || y > CLIP)
        return;
    select_font(font);
    MoveTo(x, y + text_metric[font].ascent);
    DrawText(s, 0, len);
}

int
text_width(int font, char *s)
{
    select_font(font);
    return TextWidth(s, 0, strlen(s));
}

void
get_height_width(int *h, int *w, int font, char *s)
{
    *h = text_metric[font].ascent + text_metric[font].descent;
    select_font(font);
    *w = TextWidth(s, 0, strlen(s));
}

// The Mac fills inside the rectangle:
//
//	y - 1 ->
//	         |x|x|x|
//	y     ->
//	         |x|x|x|
//	y + 1 ->
//	         |x|x|x|
//	y + 2 ->
//	         ^     ^
//	         x    x+2

void
draw_point(int x, int y)
{
    Rect r;
    r.left = x - 1;
    r.top = y - 1;
    r.right = x + 2;
    r.bottom = y + 2;
    PaintRect(&r);
}

void
draw_box(int x1, int y1, int x2, int y2)
{
    RGBColor pen;
    Rect r;
    pen.red = (256 * 16 * 16) - 1;
    pen.green = (256 * 16 * 16) - 1;
    pen.blue = (256 * 16 * 15) - 1;
    RGBForeColor(&pen);
    r.left = x1;
    r.top = y1;
    r.right = x2 + 1;
    r.bottom = y2 + 1;
    PaintRect(&r);
    pen.red = 0;
    pen.green = 0;
    pen.blue = 0;
    RGBForeColor(&pen);
    FrameRect(&r);
}

void
draw_line(int x1, int y1, int x2, int y2)
{
    if (x1 < -CLIP) x1 = -CLIP;
    if (x1 > +CLIP) x1 = +CLIP;
    if (x2 < -CLIP) x2 = -CLIP;
    if (x2 > +CLIP) x2 = +CLIP;
    if (y1 < -CLIP) y1 = -CLIP;
    if (y1 > +CLIP) y1 = +CLIP;
    if (y2 < -CLIP) y2 = -CLIP;
    if (y2 > +CLIP) y2 = +CLIP;
    MoveTo(x1, y1);
    LineTo(x2, y2);
}

// The Mac draws the last point:
//
//	|x|x|x|x|x|
//	^       ^
//	x      x+w
//
// If w = 4 then 5 pixels are drawn.
//
// To draw an hrule w pixels wide, the endpoint is x + w - 1.

void
draw_hrule(int x, int y, int w)
{
    draw_line(x, y, x + w - 1, y);
}

void
draw_left_bracket(int x, int y, int w, int h)
{
    MoveTo(x + w - 1, y);
    LineTo(x, y);
    LineTo(x, y + h - 1);
    LineTo(x + w - 1, y + h - 1);
}

void
draw_right_bracket(int x, int y, int w, int h)
{
    MoveTo(x, y);
    LineTo(x + w - 1, y);
    LineTo(x + w - 1, y + h - 1);
    LineTo(x, y + h - 1);
}

void
use_normal_pen()
{
}

void
use_graph_pen()
{
}

static void
update_scroll_bars(void)
{
    SetControl32BitMaximum(hscroll, max_x);
    SetControl32BitValue(hscroll, display_x);
    SetControl32BitMaximum(vscroll, max_y);
    SetControl32BitValue(vscroll, display_y);
}

static void
update_display(void)
{
    if (update_display_request == 0)
        return;
    draw_display_now();
}

static void draw_display_now_f(void);

static void
draw_display_now(void)
{
    RgnHandle rgn;
    SetPortWindowPort(gwindow);
    rgn = NewRgn();
    GetClip(rgn);
    draw_display_now_f();
    SetClip(rgn);
    DisposeRgn(rgn);
    update_scroll_bars();
}

static void
draw_display_now_f(void)
{
    Rect r;
    r.left = 0;
    r.top = 0;
    r.right = display_width;
    r.bottom = display_height;
    ClipRect(&r);
    EraseRect(&r);
    draw_display();
    select_font(DEFAULT_FONT);
}

static void
vscroll_f(ControlRef ref, ControlPartCode part)
{
    int dy, y;
    Rect r;
    RgnHandle rgn, tmp;

    y = display_y;

    switch (part) {

    case kControlUpButtonPart:
        display_y -= line_height;
        break;

    case kControlDownButtonPart:
        display_y += line_height;
        break;

    case kControlPageUpPart:
        display_y -= display_height;
        break;

    case kControlPageDownPart:
        display_y += display_height;
        break;

    case 129: // thumb
        display_y = GetControl32BitValue(vscroll);
        break;
    }

    if (display_y < 0)
        display_y = 0;

    if (display_y > max_y)
        display_y = max_y;

    dy = y - display_y;

    if (dy == 0)
        return;

    SetPortWindowPort(gwindow);

    rgn = NewRgn();

    if (abs(dy) >= display_height) {

        SetRectRgn(rgn, 0, 0, display_width, display_height);
        EraseRgn(rgn);

    } else {

        r.left = 0;
        r.top = 0;
        r.right = display_width;
        r.bottom = display_height;

        ScrollRect(&r, 0, dy, rgn);
    }

    tmp = NewRgn();
    GetClip(tmp);

    SetClip(rgn);
    DisposeRgn(rgn);

    draw_display();
    select_font(DEFAULT_FONT);

    SetClip(tmp);
    DisposeRgn(tmp);

    update_scroll_bars();
}

static void
hscroll_f(ControlRef ref, ControlPartCode part)
{
    int dx, x;
    Rect r;
    RgnHandle rgn, tmp;

    x = display_x;

    switch (part) {

    case kControlUpButtonPart:
        display_x -= line_height;
        break;

    case kControlDownButtonPart:
        display_x += line_height;
        break;

    case kControlPageUpPart:
        display_x -= display_width;
        break;

    case kControlPageDownPart:
        display_x += display_width;
        break;

    case 129: // thumb
        display_x = GetControl32BitValue(hscroll);
        break;
    }

    if (display_x < 0)
        display_x = 0;

    if (display_x > max_x)
        display_x = max_x;

    dx = x - display_x;

    if (dx == 0)
        return;

    SetPortWindowPort(gwindow);

    rgn = NewRgn();

    if (abs(dx) >= display_height) {

        SetRectRgn(rgn, 0, 0, display_width, display_height);
        EraseRgn(rgn);

    } else {

        r.left = 0;
        r.top = 0;
        r.right = display_width;
        r.bottom = display_height;

        ScrollRect(&r, dx, 0, rgn);
    }

    tmp = NewRgn();
    GetClip(tmp);

    SetClip(rgn);
    DisposeRgn(rgn);

    draw_display();
    select_font(DEFAULT_FONT);

    SetClip(tmp);
    DisposeRgn(tmp);

    update_scroll_bars();
}

static void
do_main_help(int n)
{
    if (running)
        return;
    SelectWindow(gwindow);
    do_help(n);
    update_display();
}

static char *inp;

static OSStatus
task(void *p)
{
    run(program_buf);
    running = 2;
    send_user_event();
    return noErr;
}

static void
create_task(void)
{
    MPTaskID id;
    DeactivateControl(inputcontrol);
    update_curr_cmd("");
    timer = time(NULL);
    running = 1;
    MPCreateTask(
        task,
        NULL,
        1024 * 1024,
        NULL,
        NULL,
        NULL,
        0,
        &id);
}

extern void update_cmd_history(char *);
extern void echo_input(char *);

static void
do_return_key(void)
{
    if (running)
        return;
    if (inp)
        free(inp);
    inp = get_curr_cmd();
    update_cmd_history(inp); // reset history pointer no matter what
    if (*inp == 0)
        return;
    strcpy(program_buf, inp);
    echo_input(program_buf);
    create_task();
}

static void
do_button(char *s)
{
    if (running)
        return;
    if (inp)
        free(inp);
    inp = get_curr_cmd();
    update_cmd_history(inp); // reset history pointer no matter what
    if (*inp == 0) {
            strcpy(program_buf, s);
            strcat(program_buf, "(last)");
    } else {
        strcpy(program_buf, s);
        strcat(program_buf, "(");
        strcat(program_buf, inp);
        strcat(program_buf, ")");
    }
    update_cmd_history(program_buf);
    echo_input(program_buf);
    create_task();
}

static void
do_run_button(void)
{
    if (running)
        return;
    get_script();
    clear();
    create_task();
}

static int shunted;

static void
deactivate_controls(void)
{
    int i;
    if (shunted == 0) {
        for (i = 0; i < 12; i++)
            DeactivateControl(buttons[i]);
        shunted = 1;
    }
}

static void
activate_controls(void)
{
    int i;
    if (shunted == 1) {
        for (i = 0; i < 12; i++)
            ActivateControl(buttons[i]);
        shunted = 0;
    }
}

static void
send_user_event(void)
{
    EventRef event;
    CreateEvent(NULL, kEventClassKeyboard, 1234, 0, kEventAttributeUserEvent, &event);
    PostEventToQueue(GetMainEventQueue(), event, kEventPriorityStandard);
}

static void
process_user_event(void)
{
    unsigned int dt;
    static char buf[1000];

    if (running == 0)
        return;

    if (running == 2) {
        activate_controls();
        ActivateControl(inputcontrol);
        update_curr_cmd("");
        update_display();
        running = 0;
        return;
    }

    dt = time(NULL) - timer;

    if (dt > 1) {
        deactivate_controls();
        sprintf(buf, "Working on it for %d seconds. Esc might interrupt, otherwise press \021Q to quit.", dt);
        update_curr_cmd(buf);
        update_display();
    }
}

static void
get_script(void)
{
    int i;
    Size len;
    if (edit_control == 0) {
        program_buf[0] = 0;
        return;
    }
    GetControlData(edit_control, 0, kControlEditTextTextTag, MAX_PROGRAM_SIZE, program_buf, &len);
    program_buf[len] = 0;
    for (i = 0; i < len; i++)
        if (program_buf[i] == '\r')
            program_buf[i] = '\n';
}

static void erase_window_f(void);

static void
erase_window(void)
{
    RgnHandle rgn;
    SetPortWindowPort(gwindow);
    rgn = NewRgn();
    GetClip(rgn);
    erase_window_f();
    SetClip(rgn);
    DisposeRgn(rgn);
}

static void
erase_window_f(void)
{
    Rect r;
    r.left = 0;
    r.top = 0;
    r.right = client_width + grow_dim;
    r.bottom = client_height + grow_dim;
    ClipRect(&r);
    EraseRect(&r);
}

static void
do_resize(void)
{
    int i;
    Rect r;
    GetWindowBounds(gwindow, kWindowContentRgn, &r);
    client_height = r.bottom - r.top - grow_dim;
    client_width = r.right - r.left - grow_dim;
    display_width = client_width - scroll_bar_dim;
    display_height = client_height - 2 * line_height - input_control_height - scroll_bar_dim;
    //KillControls(gwindow); // cannot get it to work, new controls are never drawn
    DisposeControl(hscroll);
    DisposeControl(vscroll);
    DisposeControl(inputcontrol);
    for (i = 0; i < 12; i++)
        DisposeControl(buttons[i]);
    erase_window();
    create_scroll_bar_controls();
    create_input_control();
    create_buttons();
    SetKeyboardFocus(gwindow, inputcontrol, kControlFocusNextPart);
    if (running) {
        shunted = 0;
        deactivate_controls();
        DeactivateControl(inputcontrol);
    }
    DrawControls(gwindow);
    // display_y ranges from 0 to max_y
    max_y = total_h - display_height;
    if (max_y < 0)
        max_y = 0;
    if (display_y > max_y)
        display_y = max_y;
    draw_display_now();
}

static void
copy_display(void)
{
    Rect r;
    PicHandle pic;
    RgnHandle rgn;
    ScrapRef scrap;

    r.top = 0;
    r.left = 0;
    r.right = display_width;
    if (total_h - display_y < display_height)
        r.bottom = total_h - display_y;
    else
        r.bottom = display_height;

    SetPortWindowPort(gwindow);
    rgn = NewRgn();
    GetClip(rgn);

    pic = OpenPicture(&r);

    ClipRect(&r);
    //EraseRect(&r);
    draw_display();
    select_font(DEFAULT_FONT);

    ClosePicture();

    SetClip(rgn);
    DisposeRgn(rgn);

    ClearCurrentScrap();
    GetCurrentScrap(&scrap);

    PutScrapFlavor(
        scrap,
        kScrapFlavorTypePicture,
        kScrapFlavorMaskNone,
        GetHandleSize((char **) pic),
        *pic);

    KillPicture(pic);
}

static void
copy_tty(void)
{
    char *s;
    ScrapRef scrap;
    s = get_tty_buf();
    if (s == NULL)
        return;
    ClearCurrentScrap();
    GetCurrentScrap(&scrap);
    PutScrapFlavor(
        scrap,
        kScrapFlavorTypeText,
        kScrapFlavorMaskNone,
        strlen(s),
        s);
    free(s);
}
