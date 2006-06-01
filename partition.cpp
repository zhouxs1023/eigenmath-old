/* Partition a term

	Input stack:

		term (factor or product of factors)

		free variable

	Output stack:

		constant expression

		variable expression
*/

void
partition(void)
{
	save();

	p2 = pop();
	p1 = pop();

	p3 = symbol(NIL);
	p4 = symbol(NIL);

	p1 = cdr(p1);

	while (iscons(p1)) {
		if (find(car(p1), p2)) {
			if (p4 == symbol(NIL))
				p4 = car(p1);
			else {
				push(p4);
				push(car(p1));
				multiply();
				p4 = pop();
			}
		} else {
			if (p3 == symbol(NIL))
				p3 = car(p1);
			} else {
				push(p3);
				push(car(p1));
				multiply();
				p3 = pop();
			}
		}
		p1 = cdr(p1);
	}

	if (p3 == symbol(NIL))
		push_integer(1);
	else
		push(p3);

	if (p4 == symbol(NIL))
		push_integer(1);
	else
		push(p4);

	restore();
}
