#include "Main.h"

//頭とアイテムの衝突判定関数
BOOLEAN ItemHead(BOOLEAN ItemLive, int _y, int item_y, int itemSize, int Size, int _x, int Size2, int item_x){
	if (ItemLive == TRUE){
		if (_y <= item_y + itemSize && _y + Size >= item_y + itemSize && _x + Size2 >= item_x &&  _x + Size2 <= item_x + itemSize){
			return TRUE;
		}
	}
	return FALSE;
	//if (ItemLive == FALSE)
		//return FALSE;
//	return (_y <= item_y + itemSize && _y + Size >= item_y + itemSize && _x + Size2 >= item_x &&  _x + Size2 <= item_x + itemSize);
}

//横とアイテムの左側の衝突判定の関数
BOOLEAN ItemSide(BOOLEAN ItemLive, int _x, int Size, int item_x, int Size2, int _y, int item_y, int itemSize){
	if (ItemLive == TRUE){
		if (_x + Size >= item_x && _x + Size2 <= item_x &&  _y <= item_y + itemSize && _y + Size >= item_y){
			return TRUE;
		}
	}
	return FALSE;
}