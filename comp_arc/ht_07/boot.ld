SECTIONS
{
	. = 0x7C00;
	
	.text :
	{
		*(.text)
	}

	. = 0x7C00 + 508;

	.data :
	{
		*(.data)
	}
}