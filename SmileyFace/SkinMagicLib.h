/***************************************************************************

// This is a part of the SkinMagic SDK library.
// Copyright (C) 2009-2002 Appspeed Inc.
// All rights reserved.
//
// This source code is only intended as a supplement to the
// SkinMagic SDK Reference and related electronic documentation 
// provided with the library.
// See these sources for detailed information regarding the
// SkinMagic SDK product.

// SkinMagicLib SDK is a library designed for C/C++ programmer, 
// you can add the most popular function of changing skin to your
// application with the API supplied by this library. SkinMagic 
// is picture-based skin system, you can power your application
// with skin-changeable with simple call of several functions. 
// And with the skin editor of SkinMagicBuilder, you can customize
// your own skin.

*****************************************************************************/
#ifndef _SKINMAGICLIB_H__
#define _SKINMAGICLIB_H__

#include <windows.h>
#include <commctrl.h>

//Skinmagic error code
#define   SMEC_OK                      0
#define   SMEC_REGISTERWINDOW	       1    //fail to register windows class
#define   SMEC_MESSAGEHOOK             2    //fail to set message hook    
#define   SMEC_KEYHOOK				   3	//fail to set keyboard hook  
#define   SMEC_FILENOTFOUND            4    //not find the file
#define   SMEC_DECOMPRESSERROR         5    //fail to decompress skin file 
#define   SMEC_HEADERERROR             6    //skin file header error
#define   SMEC_SKINVERSIONERROR        7    //skin file version not match 
#define   SMEC_OBJECTNOTFOUND          8    //obect not found
#define   SMEC_OBJECTTYPEUNMATCH       9    //Object type not match
#define   SMEC_IMAGESIZEERROR          10   //Image size error
#define   SMEC_CREATEIMAGELISTFAIL     11   //fail to create image list object 
#define   SMEC_NOTIMPLEMENT            12   //The function not implement
#define   SMEC_PARAMETERERROR          13   //the function parameter error 
#define   SMEC_INITSCROLLBARFAIL       14   //fail to skin scrollbar
#define   SMEC_LOADRESOURCEFAIL        15   //fail to load the resource
#define   SMEC_FINDRESOURCEFAIL        16   //fail to find the resource
#define   SMEC_WINDOWSXPNOTCOMPATIBLE  17   //not compatible with Windows XP
#define   SMEC_OUTOFMEMORY             18   //Out of memory
#define   SMEC_LIBARAYNOTINITIALIZED   19   //Library not initialized

#pragma pack(push, 8)
///////////////////////////////////////////////////////////////////////////
//Function: InitSkinMagicLib(HINSTANCE hInstance, LPCTSTR szApplication , 
//					  LPCTSTR szReserved1,
//					  LPCTSTR szReserved2
//			
//Parameters: 
//		hInstance 
//			[in] Handle of application instance.
//		szApplication
//			[in] Application defined in skin file. If you set this parameter the 
//			library can only load the skin file which application object named
//			match szApplication.
//		szReserved1
//			[in] Reserved.
//		szReserved2
//			[in] Reserved.
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
//Remarks:
//	 InitSkinMagicLib function should be the first API called in SkinMagicLib SDK,
//	 followed by other APIs. 
//	 For multithreaded applications, you must call InitSkinMagicLib in every thread. 
//   
int  __stdcall InitSkinMagicLib( HINSTANCE hInstance, 
								 LPCTSTR lpApplication , 
								 LPCTSTR lpReserved1,
								 LPCTSTR lpReserved2 );

///////////////////////////////////////////////////////////////////////////
//Function: LoadSkinFile( LPCTSTR SkinFile )
//			Load skin from file.
//Parameters: 
//		SkinFile 
//			[in] Specifies the skin file name. 
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
//Remarks:
//	 ExitSkinMagicLib function should be the last API called in SkinMagicLib SDK.
//	 For multithreaded applications, you must call InitSkinMagicLib in every thread. 
void  __stdcall ExitSkinMagicLib();

///////////////////////////////////////////////////////////////////////////
//Function: LoadSkinFile( LPCTSTR SkinFile )
//			Load skin from file.
//Parameters: 
//		SkinFile 
//			[in] Specifies the skin file name. 
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
//Remarks:
//		Load skin from file.	
int  __stdcall LoadSkinFile( LPCTSTR lpSkinFile );

///////////////////////////////////////////////////////////////////////////
//Function: LoadSkinFromResource(HMODULE hModule, LPCTSTR lpSkinName ,LPCTSTR lpType)
//			Use this function to load skin from resource.
//Parameters: 
//		hModule 
//			[in] Handle to the module whose executable file contains the resource. 
//			A value of NULL specifies the module handle associated with the image 
//			file that the operating system used to create the current process. 
//		lpSkinName 
//			[in] Specifies the name of the skin resource. 
//		lpType 
//			[in] Specifies the resource type. 

//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
//Remarks:
//		The application can use skin defined in resource.
int  __stdcall LoadSkinFromResource(HMODULE hModule, LPCTSTR lpSkinName ,LPCTSTR lpType);

///////////////////////////////////////////////////////////////////////////
//Function: SetWindowSkin( HWND hWnd , LPCTSTR SkinName )
//			Use SetWindowSkin to skin the standard windows - with rectangular
//          opaque client area and region-based non-client area, menu bar, 
//          system/maximize/minimize/close buttons, caption and sizing edges
//          and corners. 
//			
//Parameters: 
//   hWnd
//		[in]Handle to the window to be remove skin.     
//   SkinName
//		[in]Specifies the name of FrameWnd object which defined in skin file. 
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
//Remarks:
//			Use SetWindowSkin to skin the standard windows - with rectangular
//          opaque client area and region-based non-client area, menu bar, 
//          system/maximize/minimize/close buttons, caption and sizing edges
//          and corners.
int  __stdcall SetWindowSkin( HWND hWnd , LPCTSTR lpSkinName );


///////////////////////////////////////////////////////////////////////////
//Function: SetShapeWindowSkin( HWND hWnd , LPCTSTR SkinName )
//			Use SetShapeWindowSkin to skin application-specific windows 
//			without non-client area and menu bar. These windows can have
//			absolutely any shape or even be translucent. 
//			
//Parameters: 
//   hWnd
//		[in]Handle to the window to be remove skin.     
//   SkinName
//		[in]Specifies the name of ShapeWnd object which defined in skin file. 
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
//Remarks:
//	 Use SetShapeWindowSkin to skin application-specific windows 
//	 without non-client area and menu bar. These windows can have
//	 absolutely any shape or even be translucent.
//
int  __stdcall SetShapeWindowSkin( HWND hWnd , LPCTSTR SkinName );


///////////////////////////////////////////////////////////////////////////
//Function: RemoveWindowSkin( HWND hWnd )
//			set skin of new create dialog
//			
//Parameters: 
//   hWnd
//		[in]Handle to the window to be remove skin.      
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
//Remarks:
//	 Use this function to remove window skin.
//
int  __stdcall RemoveWindowSkin( HWND hWnd );


///////////////////////////////////////////////////////////////////////////
//Function: SetDialogSkin( LPCTSTR szSkinName )
//			set skin of new create dialog
//			
//Parameters: 
//   szSkinName
//      [in]Specifies the name of FrameWnd object which defined in skin file.      
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
//Remarks:
//	 Use this function to set dialog skin. dialog will auto be skinned
//   after this function called.
//
int  __stdcall SetDialogSkin( LPCTSTR szSkinName );

///////////////////////////////////////////////////////////////////////////
//Function: SetSingleDialogSkin( HWND hWnd ,LPCTSTR szSkinName )
//			Set the skin of an dialog
//			
//Parameters: 
//   hWnd
//		[in]Handle to the dialog to be set skin. 
//   szSkinName
//      [in]Specifies the skin nanem.      
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
//Remarks:
//	 Use this function to set single dialog skin.
//
int  __stdcall SetSingleDialogSkin( HWND hWnd , LPCTSTR szSkinName );

///////////////////////////////////////////////////////////////////////////
//Function: RemoveDialogSkin()
//			This function will stop auto skin dialog. 
//			the function will not remove the dialog's
//          skin which already set. The new create dialog will have no skin.
//			
//Parameters: 
//			none.
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
//Remarks:
//	 Use this function to stop auto skin dialog.
//
int  __stdcall RemoveDialogSkin();


///////////////////////////////////////////////////////////////////////////
//Function: EnableWindowScrollbarSkin( HWND hWnd ,int fnBar )
//			Initialize the skin scrollbars for a window
//			
//Parameters: 
//   hWnd
//		[in]Handle to the window to be set skin scrollbar. 
//   fnBar
//      [in]Specifies the type of scroll bar for which to skin. 
//		This parameter can be one of the following values       
//		SB_HORZ for the window's standard horizontal scroll bar.  
//		SB_VERT for the window's standard  scroll bar.  
//		SB_BOTH for both horizontal and vertical scroll bar.   
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
//Remarks:
//	 Use this function to skin your window scrollbar
//
int  __stdcall EnableWindowScrollbarSkin( HWND hWnd ,int fnBar );


///////////////////////////////////////////////////////////////////////////
//Function: DisableWindowScrollbarSkin( HWND hWnd )
//			disable the skin scrollbars for a window
//			
//Parameters: 
//   hWnd
//		[in]Handle to the window to be disable skin scrollbar. 
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
//Remarks:
//	 Use this function to skin your window scrollbar
//
int  __stdcall DisableWindowScrollbarSkin( HWND hWnd );

///////////////////////////////////////////////////////////////////////////
//Function: TrackSkinPopupMenu( HMENU hMenu,int x, int y, HWND hWnd )
//			The TrackSkinPopupMenu function displays a shortcut menu at the 
//			specified location and tracks the selection of items on the menu.
//			The shortcut menu can appear anywhere on the screen.
//			
//Parameters
//		hMenu 
//			[in] Handle to the shortcut menu to be displayed. 
//				 The handle can be obtained by calling CreatePopupMenu
//				 to create a new shortcut menu, or by calling GetSubMenu
//				 to retrieve a handle to a submenu associated with an 
//				 existing menu item. 
//uFlags 
//			[in] Use zero of more of these flags to specify function options. 
//			Use one of the following flags to specify how the function positions
//			the shortcut menu horizontally. 
//			------------------------------------------------------------------------------
//			TPM_CENTERALIGN		If this flag is set, the function centers the shortcut
//								menu horizontally relative to the coordinate specified
//								by the x parameter. 
//			TPM_LEFTALIGN		If this flag is set, the function positions the shortcut
//								menu so that its left side is aligned with the coordinate
//								specified by the x parameter. 
//			TPM_RIGHTALIGN		Positions the shortcut menu so that its right side is 
//								aligned with the coordinate specified by the x parameter. 
//			------------------------------------------------------------------------------
//			Use one of the following flags to specify how the function positions the shortcut
//			menu vertically. 
//			TPM_BOTTOMALIGN		If this flag is set, the function positions the shortcut 
//								menu so that its bottom side is aligned with the coordinate 
//								specified by the y parameter. 
//			TPM_TOPALIGN		If this flag is set, the function positions the shortcut
//								menu so that its top side is aligned with the coordinate 
//								specified by the y parameter. 
//			TPM_VCENTERALIGN	If this flag is set, the function centers the shortcut menu 
//								vertically relative to the coordinate specified by the y parameter. 
//
//		x 
//			[in] Specifies the horizontal location of the shortcut menu, 
//				 in screen coordinates. 
//		y 
//			[in] Specifies the vertical location of the shortcut menu, 
//				 in screen coordinates. 
//		hWnd 
//			[in] Handle to the window that owns the shortcut menu. 
//				 This window receives all messages from the menu. 
//				 The window does not receive a WM_COMMAND message from the
//				 menu until the function returns. 
BOOL  __stdcall TrackSkinPopupMenu(  HMENU hMenu, UINT uFlags, int x, int y, HWND hWnd );


int __stdcall EnableCaptionButtons(HWND hWnd , LPCTSTR lpName , BOOL bEnable );

//for custom control support
#define WM_SKINDATACHANGED     WM_USER + 2000

//for 
#define WM_SETSKIN             WM_APP  + 1000

typedef HANDLE HSKIN;          // handle to a skin data object


int __stdcall RegisterSkinWindow( HWND hWnd , UINT uNotifyMsg );
int __stdcall UnregisterSkinWindow( HWND hWnd );  

HSKIN __stdcall OpenSkinData( LPCTSTR pszSkinObjectName );
void  __stdcall CloseSkinData( HSKIN hSkin );

//stocked property for every custom control
int __stdcall GetSkinControlRect( HSKIN hSkin , RECT* pRect, BOOL* pAutoPlace );
int __stdcall GetSkinControlID( HSKIN hSkin , DWORD* dwID );
int __stdcall GetSkinControlFont( HSKIN hSkin , HFONT* phFont , COLORREF* pColor );
int __stdcall GetSkinControlColor( HSKIN hSkin , COLORREF* pColor );
int __stdcall GetSkinControlBkColor( HSKIN hSkin , COLORREF* pColor );
int __stdcall GetSkinTransparentColor( COLORREF* pColor , BOOL* pBool );

//user customed property                     
int __stdcall GetSkinFont( HSKIN hSkin,LPCTSTR pszName , LOGFONT* pLogFont , COLORREF* pColor );
int __stdcall GetSkinColor( HSKIN hSkin , LPCTSTR pszName, COLORREF* pColor );
int __stdcall GetSkinBool( HSKIN hSkin ,  LPCTSTR pszName, bool* pbVal );
int __stdcall GetSkinInt( HSKIN hSkin ,  LPCTSTR pszName, int* piVal );
int __stdcall GetSkinDWORD( HSKIN hSkin ,  LPCTSTR pszName, DWORD* pdwVal );
int __stdcall GetSkinEnum( HSKIN hSkin ,  LPCTSTR pszName, DWORD* pdwVal );
int __stdcall GetSkinString( HSKIN hSkin, LPCTSTR pszName , LPTSTR pszBuff , unsigned long* pMaxBuffChars );
int  __stdcall DrawSkinImageRect( HSKIN hSkin,
								   LPCTSTR pszName,
								   HDC hdc,
								   RECT* pRect
								   );

int  __stdcall DrawSkinImageSection( HSKIN hSkin, 
									   LPCTSTR pszName,
									   HDC hdc,
									   RECT* pRect );

int  __stdcall DrawSkinImageSectionFrame( HSKIN hSkin, 
									   LPCTSTR pszName,
									   HDC hdc,
									   RECT* pRect );

int __stdcall GetSkinImageSectionMargins( HSKIN hSkin, LPCTSTR pszName , const RECT* pRect , const RECT* pMargins );   

int __stdcall CreateSkinImageRectRegion( HSKIN hSkin,
										 LPCTSTR pszName,
										 RECT* pBoundRect, 
										 SIZE* pImgSize,
										 HRGN* pRegion);

int __stdcall CreateSkinImageSectionRegion( HSKIN hSkin,
											LPCTSTR pszName,
											RECT* pRect,
											HRGN* pRegion );

                       
int __stdcall DrawSkinTextEffect( HSKIN hSkin, 
                            LPCTSTR pszName,
							HDC   hdc,
                            const RECT* pRect,
							LPCTSTR lpText,
							UINT  uLen,
                            UINT  uFlag );  

///////////////////////////////////////////////////////////////////////////////////////////////
//CreateImageList
//
//The CreateImageList create a new image list from skin file. 
//
//   Parameters:
//       lpImagelistObjectName [in] The skinImageList object name.
//   Return Values
//       Returns the handle to the image list if successful, or NULL otherwise
///////////////////////////////////////////////////////////////////////////////////////////////
HIMAGELIST __stdcall CreateImageList( LPCTSTR pszImageListObjectName );


struct MENUIMAGEINFO
{
	UINT itemID;
	int  iImage;
};


//////////////////////////////////////////////////////////////////////////////////////////////////
//Function: SetWindowMainMenuImage
//			The SetWindowMainMenuImage function set windows main menu item's bitmap.
//			
//Parameters: 
//	hWnd
//		[in] Handle to the window own the menu..
//	pMenuImage
//		[in] Pointer of an array of MENUIMAGEINFO struct .
//	nCount
//		[in] length of array pMenuImage.
//	hImageList
//		[in] imagelist used by the main menu to display normal item images. 
//	hHotImageList
//		[in] imagelist used by the main menu to display hot item images. 
//	hDisabledImagelist
//		[in] imagelist used by the main menu to display disabled item images.    
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
///////////////////////////////////////////////////////////////////////////////////////////
int __stdcall SetWindowMainMenuImage( HWND hWnd ,
									  MENUIMAGEINFO* pMenuImage, 
									  UINT nCount , 
									  HIMAGELIST hImageList , 
									  HIMAGELIST hHotImageList,
									  HIMAGELIST hDisabledImagelist );



/////////////////////////////////////////////////////////////////////////////////////////////
//Function: TrackSkinPopupMenuEx
//			The TrackSkinPopupMenuEx function displays a shortcut menu at the 
//			specified location and tracks the selection of items on the menu.
//			The shortcut menu can appear anywhere on the screen.
//			
//Parameters
//		hMenu 
//			[in] Handle to the shortcut menu to be displayed. 
//				 The handle can be obtained by calling CreatePopupMenu
//				 to create a new shortcut menu, or by calling GetSubMenu
//				 to retrieve a handle to a submenu associated with an 
//				 existing menu item. 
//		lpSkinPopupMenuName
//			[in] The SkinPopupMenu object name. If lpSkinPopupMenuName is NULL SkinMagic 
//			use default popupmenu skindata.
//		uFlags 
//			[in] Use zero of more of these flags to specify function options. 
//			Use one of the following flags to specify how the function positions
//			the shortcut menu horizontally. 
//			------------------------------------------------------------------------------
//			TPM_CENTERALIGN		If this flag is set, the function centers the shortcut
//								menu horizontally relative to the coordinate specified
//								by the x parameter. 
//			TPM_LEFTALIGN		If this flag is set, the function positions the shortcut
//								menu so that its left side is aligned with the coordinate
//								specified by the x parameter. 
//			TPM_RIGHTALIGN		Positions the shortcut menu so that its right side is 
//								aligned with the coordinate specified by the x parameter. 
//			------------------------------------------------------------------------------
//			Use one of the following flags to specify how the function positions the shortcut
//			menu vertically. 
//			TPM_BOTTOMALIGN		If this flag is set, the function positions the shortcut 
//								menu so that its bottom side is aligned with the coordinate 
//								specified by the y parameter. 
//			TPM_TOPALIGN		If this flag is set, the function positions the shortcut
//								menu so that its top side is aligned with the coordinate 
//								specified by the y parameter. 
//			TPM_VCENTERALIGN	If this flag is set, the function centers the shortcut menu 
//								vertically relative to the coordinate specified by the y parameter. 
//
//		x 
//			[in] Specifies the horizontal location of the shortcut menu, 
//				 in screen coordinates. 
//		y 
//			[in] Specifies the vertical location of the shortcut menu, 
//				 in screen coordinates. 
//		hWnd 
//			[in] Handle to the window that owns the shortcut menu. 
//				 This window receives all messages from the menu. 
//				 The window does not receive a WM_COMMAND message from the
//				 menu until the function returns. 
//		pMenuImage
//			[in] Pointer of an array of MENUIMAGEINFO struct .
//		nCount
//			[in] length of array pMenuImage.
//		hImageList
//			[in] imagelist used by the main menu to display normal item images. 
//		hHotImageList
//			[in] imagelist used by the main menu to display hot item images. 
//		hDisabledImagelist
//			[in] imagelist used by the main menu to display disabled item images.  
///////////////////////////////////////////////////////////////////////////////////////////
BOOL  __stdcall TrackSkinPopupMenuEx(  HMENU hMenu,
									   LPCTSTR lpSkinName,
									   UINT uFlags, 
									   int x, 
									   int y, 
									   HWND hWnd ,
									   MENUIMAGEINFO* pMenuImage, 
									   UINT nCount , 
									   HIMAGELIST hImageList , 
									   HIMAGELIST hHotImageList,
									   HIMAGELIST hDisabledImagelist );                     

int __stdcall RedrawCaptionStatic( HWND hWnd , LPCTSTR lpName );

DWORD __stdcall GetSkinMagicErrorCode();

//menu function
//Use these function to get and set skinned window's menu
HMENU __stdcall GetSkinMenu( HWND hWnd );
BOOL __stdcall SetSkinMenu( HWND hWnd , HMENU hMenu );


//The state of caption button
#define CPBS_HOVERED    0x00000000
#define CPBS_PRESSED    0x00000001
#define CPBS_DISABLED   0x00000002
#define CPBS_NORMAL		0x00000003

//////////////////////////////////////////////////////////////////////////////////////////////////
//Function: GetCaptionButtonState
//			The GetCaptionBttonState function get captionbutton object's state.
//			
//Parameters: 
//	hWnd
//		[in] Handle to the window own the captionbutton.
//	lpName
//		[in] the captionbutton's name 
//	pdwState
//		[out] the state of captionbutton.    
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
///////////////////////////////////////////////////////////////////////////////////////////
int __stdcall GetCaptionButtonState(HWND hWnd , LPCTSTR lpName , DWORD* pdwState );

//////////////////////////////////////////////////////////////////////////////////////////////////
//Function: SetCaptionButtonState
//			The SetCaptionBttonState function set captionbutton object's state.
//			
//Parameters: 
//	hWnd
//		[in] Handle to the window own the captionbutton.
//	lpName
//		[in] the captionbutton's name 
//	dwState
//		[in] the state of captionbutton.    
//
//Return Values:
//   If the function success, the return value is nonzero
//   If the function fail, the return value is zero
///////////////////////////////////////////////////////////////////////////////////////////
int __stdcall SetCaptionButtonState(HWND hWnd , LPCTSTR lpName , DWORD dwState );


///////////////////////////////////////////////////////////////////////////////////////////
int __stdcall SetSkinWindowAccelerator( HWND hWnd, HACCEL hAccTable );


////////////////////////////////////////////////////////////////////////////////////////////
int __stdcall SetControlTooltip( HWND hWnd , LPCTSTR lpTooltip );

int __stdcall SetCaptionButtonTooltip( HWND hWnd , LPCTSTR lpName , LPCTSTR lpTooltip );

int __stdcall ShowTooltipPoint( HWND hWnd , LPCTSTR lpTooltip , LPPOINT lpPoint );

int __stdcall HideTooltip();


int __stdcall SetControlSkin( HWND hWnd , LPCTSTR szSkinName, LPCTSTR szOwnerSkinName  );
COLORREF __stdcall GetSkinSysColor( int nIndex );
HBRUSH __stdcall GetSkinSysColorBrush( int nIndex );

//
int __stdcall ShowSkinObject( HWND hWnd , LPCTSTR szSkinObject , BOOL bVisible );

int __stdcall SetSkinObjectText( HWND hWnd , LPCTSTR szSkinObject , LPCTSTR lpText );

int __stdcall GetSkinObjectText( HWND hWnd , LPCTSTR szSkinObject , LPTSTR  lpText , int nMaxCount );

int __stdcall GetLibVersion( LPTSTR lpVersion );

int __stdcall GetSkinClientRect(  HWND hWnd, LPRECT lpRect );



#define WM_CB_MOUSEMOVE		  WM_USER + 2001
#define WM_CB_LBUTTONDOWN     WM_USER + 2002
#define WM_CB_RBUTTONDOWN	  WM_USER + 2003
#define WM_CB_LBUTTONUP		  WM_USER + 2004
#define WM_CB_RBUTTONUP		  WM_USER + 2005
#define WM_CB_LBUTTONDBLCLK   WM_USER + 2006
#define WM_CB_MOUSELEAVE	  WM_USER + 2007
#define WM_CB_NOTIFY          WM_USER + 2008 
#define WM_PAINTCLIENT        WM_USER + 2009
//wParam hDC    
//lParam lpRect - the client rect

HBITMAP __stdcall CreateBitmapFromSkinImage( LPCTSTR szSkinImageName );


#pragma pack(pop)
#endif
