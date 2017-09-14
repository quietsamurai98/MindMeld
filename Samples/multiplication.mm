,A //store user input in cell 0
-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A //convert number char to actual number
>A
,A //store user input in cell 1
-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A-A //convert number char to actual number
<A
//at this point cell 0 and cell 1 contain ascii values we want to multiply
//both heads point to cell 0

//multiplication loop
[A
	-A
	
	//copy cell 1 into cell 2 and 3
	>A    //move head a to cell 1
	>B>B  //move head b to cell 2
	[A
		-A
		+B //add 1 to cell 2
		>B
		+B //add 1 to cell 3
		<B
	]A
	
	//move cell 3 into cell 1
	>B //head a is at cell 1 and head b is at cell 3
	[B
		-B
		+A
	]B
	
	//add cell 2 to cell 4
	>A>B //move head a to cell 2 and head b to cell 4
	[A
		-A
		+B
	]A
	
	<A<A<B<B<B<B //return heads to cell 0
]A

>A>A>A>A //move head a to cell 4
.A       //print char with ascii value equal to cell 0 times cell 1


//multiplies two digits and prints char with ascii code of product
//example: 89H
//explain: 8 times 9 is 72
//         ascii val of H is 72
