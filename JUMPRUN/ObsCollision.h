#include "Main.h"

//áŠQ•¨‚Æ‰¡‚ÌÕ“Ë”»’è
BOOLEAN ObsSide(BOOLEAN obsLive, int _x, int iSize, int obs_x, int _y, int obs_y, int obsSizeY){
	if (obsLive == TRUE){
		if (_x + iSize >= obs_x && _x <= obs_x  &&  _y + iSize >= obs_y){
			if (_y + iSize >= obs_y + 5 && _y < obs_y + obsSizeY){
				return TRUE;
			}
		}
	}
	return FALSE;
}

//áŠQ•¨‚Ìã‚ÌÕ“Ë”»’è
BOOLEAN ObsTop(BOOLEAN obsLive, int _y, int size, int obs_y, int _x, int obs_x, int obssize_x){
	if (obsLive == TRUE){
		if (_y + size > obs_y && _x + size >= obs_x && _x <= obs_x + obssize_x){
			return TRUE;
		}
	}
	return FALSE;
}