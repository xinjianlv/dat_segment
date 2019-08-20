#ifndef		JUD_H
#define		JUD_H
#include "myhead.h"

short isPunctuation(wchar word)
{   
    if(  word == 0x0020  ||  //空格
		word == 0x3000   || //全角空格
		word == 0x002E   ||//.
		word == 0xFF0C   || // ，
		word == 0x3002   ||  // 。
		word == 0x003F   || // ？
		word == 0x0021   || // ！
		word == 0x201C   || //"  左双引号
		word == 0x201D   || //"	右双引号
		word == 0x2026   ||  // ……
		word == 0xFF1F   ||   //？
		word == 0xFF01   ||  //	！
		word == 0x3001   ||      // 、
		word == 0x000A   ||      //回车
		word == 0x000D   ||      //换行
		word == 0x0D0A   ||   // 换行回车
		word == 0x0A0D   ||   //回车换行
		word == 0xFF1B   ||    //；
		word == 0xFF1A   ||        //：
		word == 0x0009   ||  // 制表符
		word == 0x002C   ||          // ,
		word == 0x002D   ||  // -
		word == 0x2014   || // —
		word == 0xFF08   || // (
		word == 0xFF09   || //)
		word == 0x0029   || //)
		word == 0x0028    //(
		)
		return 1;
	
	
	return 0;

}
short isNum(wchar word)
{
	if( word == 0x96F6 ||  //零
		word == 0x58F9 ||  //壹
		word == 0x8D30 ||	  //贰
		word == 0x53C1 ||
		word == 0x8086 ||
		word == 0x4F0D ||
		word == 0xC2BD || 
		word == 0x67D2 ||
		word == 0x634C ||
		word == 0x7396 ||   //玖
		word == 0x62FE || //拾
		word == 0x4F70 || //佰
		word == 0x4EDF || //仟
		word == 0x842C  //萬
		)
		return 1;
	return 0;
}
short isnum(wchar word)
{
	if(0x002F  < word  && word < 0x003A) return 1;
	if(	word == 0x96F6 ||   //零
		word == 0x4E00 ||	   //一
		word == 0x4E8C ||   //二
		word == 0x4E09 ||   //三
		word == 0x56DB ||
		word == 0x4E94 ||
		word == 0x516D || 
		word == 0x4E03 ||
		word == 0x516B ||
		word == 0x4E5D ||    //九
		word == 0x4E07 ||     //万
		word == 0x5343 ||    //千
		word == 0x767E ||    //百
		word == 0x5341     // 十
		)  
		return 1;
	return 0;

}
short isChar(wchar word)
{
	if(0x0060 < word && word < 0x007B) 	return 1;  // a~~z
	if(0x0040 < word && word < 0x005B) return 1;  // A~~Z

	return 0;
}


#endif