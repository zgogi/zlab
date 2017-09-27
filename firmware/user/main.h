#ifndef __MAIN_H_INCLUDED
#define __MAIN_H_INCLUDED

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

#endif 
