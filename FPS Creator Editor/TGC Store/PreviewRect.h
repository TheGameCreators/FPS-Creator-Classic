// --------------
// CPreviewRect.h
// --------------
/**
* @file
* @brief Re: PreviewRect
* @author Achim Klein
* @version 0.3
*/


// -------------------
// One-Definition-Rule
// -------------------
#ifndef PREVIEWRECT_H_INCLUDE_NR1
#define PREVIEWRECT_H_INCLUDE_NR1


// -------
// Pragmas
// -------
#if _MSC_VER > 1000
#pragma once
#endif


// ------------------------------------
// Definition of the CPreviewRect class
// ------------------------------------
/**
 * This class represents a CStatic control that can display images.
 */
class CPreviewRect : public CStatic
{

public:

	// ------------
	// Construction
	// ------------

	/// standard-constructor
	CPreviewRect();

	/// virtual destructor
	virtual ~CPreviewRect();


	// --------------
	// Initialization
	// --------------

	/// loads a bmp file
	void LoadBitmap(const CString& Path);

	/// loads a bitmap resource
	void LoadBitmap(UINT nIDResource);

	/// creates a new bitmap that is equal to the passed one
	void CopyBitmap(CBitmap* pBitmap);

	/// uses the passed bitmap without duplicating its data
	void BorrowBitmap(CBitmap* pBitmap);

	/// creates a new bitmap from raw RGB data
	void CreateBitmap(int Width, int Height, unsigned char* BGRA);


	// --------
	// Handling
	// --------

	/// resizes the control's bitmap to save up memory and to speed up drawing
	void CreateThumbnail(int MaxWidth, int MaxHeight);

	/// deletes the current bitmap
	void Reset(BOOL GraphicalUpdate = TRUE);

	int GetWidth( );
	int GetHeight( );


	// -----------
	// Information
	// -----------

	/// returns TRUE if the control contains a presentable bitmap
	BOOL IsInitialized() const;


	// ---------------------
	// Message map functions
	// ---------------------
	//{{AFX_MSG(CPreviewRect)
	/// WM_PAINT handler
	afx_msg void OnPaint();
	/// WM_SIZE  handler
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


protected:

	// ----------------
	// Internal methods
	// ----------------

	/// returns the rectangle of the bitmap
	CRect doGetBitmapRect(CBitmap* pBitmap);

	/// returns the rectangle of the proportionally zoomed bitmap
	CRect doGetZoomedBitmapRect(CBitmap* pBitmap, const CRect& FitInto);

	/// returns TRUE if the visible CBitmap needs to be resized
	BOOL doCheckZoom();

	/// draws the visible CBitmap
	void doDrawBitmap();

	/// fills the area that is not covered by the visible CBitmap with the background color
	void doDrawBackground();	

	/// invalidates and updates the client area
	void doRequestGraphicalUpdate();

	/// returns a new bitmap that is equal to the passed one
	CBitmap* doCopyBitmap(CBitmap* pBitmap);

	/// returns a new bitmap that fits proportionally into the specified rectangle
	CBitmap* doZoomBitmap(CBitmap* pBitmap, const CRect& FitInto);


private:

	// ----------
	// Attributes
	// ----------

	/// the bitmap in its original (or thumbnail) size
	CBitmap* m_bitmap;

	/// the bitmap in its zoomed size
	CBitmap* m_visible;

	/// true if the bitmap is only borrowed
	bool m_borrowed;
};


//{{AFX_INSERT_LOCATION}}

#endif	// #ifndef PREVIEWRECT_H_INCLUDE_NR1
