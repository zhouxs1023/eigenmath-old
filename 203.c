/* Sample shared memory code. */

#include <sys/ipc.h>
#include <sys/shm.h>

main()
{
	int *a, i, id;

	unsigned int t;

	id = shmget(0x12345678, 4096, IPC_CREAT | SHM_R | SHM_W);

	a = (int *) shmat(id, 0, 0);

	printf("%08x %08x\n", id, a);

	a[12] = 0;

	for (;;) {

		t = time(0);
		while (time(0) - t < 2)
			;

		a[12]++;

		printf("%d\n", a[12]);
	}
}

#include <sys/ipc.h>
#include <sys/shm.h>

main()
{
	int *a, i, id;

	unsigned int t;

	id = shmget(0x12345678, 4096, IPC_CREAT | SHM_R | SHM_W);

	a = (int *) shmat(id, 0, 0);

	printf("%08x %08x\n", id, a);

	for (;;) {

		t = time(0);
		while (time(0) - t < 2)
			;

		printf("%d\n", a[12]);
	}
}
