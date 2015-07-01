#include "stdafx.h"
#include "class_f.h"

int main(void) 
{
	inf();
	while (true)
	{
		Field _2048(get_rank());
		_2048.add();
		while (_2048.add())
		{
			_2048.show_field();
			_2048.move(get_dir());
			_2048.refresh_free();
		}
		_2048.save_record_in_file();
		_2048.~Field();
		if (!_2048.wcontinue())
			break;
	}
	return 0;
}