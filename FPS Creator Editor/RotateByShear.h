#ifndef _ROTATE_BY_SHEAR_H_
#define _ROTATE_BY_SHEAR_H_

#include <math.h>

#ifndef _PROGRESS_AND_ABORT_CALL_BACK
#define _PROGRESS_AND_ABORT_CALL_BACK

typedef BOOL (*ProgressAnbAbortCallBack)(BYTE bPercentComplete);

#endif  // _PROGRESS_AND_ABORT_CALL_BACK

template <class CPxl>
class CRotateByShear
{
public:

    CRotateByShear (ProgressAnbAbortCallBack callback = NULL) : 
        m_Callback (callback) {}

    virtual ~CRotateByShear() {}

    CPxl * AllocAndRotate (  
        CPxl   *pSrc,           // Pointer to source image
        SIZE        sSrc,       // Size of source image
        double      dAngle,     // Rotation angle
        SIZE       *psDst,      // Pointer to destination image size
        COLORREF    clrBack);   // Background color

protected:

    //
    // You must override these 5 function in the derived class and implement 
    // specific pixel access functions according to the bitmap type you use.
    //

        // Get RGB value at given pixel coordinates
    virtual COLORREF GetRGB (CPxl *pImage,      // Pointer to image
                             SIZE  sImage,      // Size of image
                             UINT  x,           // X coordinate
                             UINT  y            // Y coordinate
                            ) = 0;

        // Set RGB value at given pixel coordinates
    virtual void     SetRGB (CPxl *pImage,   // Pointer to image
                             SIZE  sImage,   // Size of image
                             UINT  x,        // X coordinate
                             UINT  y,        // Y coordinate
                             CPxl  clr       // New color to set
                            ) = 0;

        // Create a new bitmap, given a bitmap dimensions
    virtual CPxl *CreateNewBitmap (SIZE  sImage // Size of image
                            ) = 0;

        // Create a new bitmap which is an identical copy of the source bitmap
    virtual CPxl *CopyBitmap (CPxl *pImage,     // Pointer to source image
                              SIZE  sImage      // Size of source (and destination) image
                             ) = 0;

        // Destroy a bitmap previously created in call to CreateNewBitmap(..)
        // or to CopyBitmap (...)
    virtual void DestroyBitmap (CPxl *pImage,   // Pointer to image
                                SIZE  sImage    // Size of image
                               ) = 0;

private:

    ProgressAnbAbortCallBack    m_Callback;

    void HorizSkew (CPxl *pSrc, 
                    SIZE sSrc,
                    CPxl *pDst, 
                    SIZE sDst,
                    UINT uRow, 
                    int iOffset, 
                    double dWeight,
                    COLORREF clrBack);

    void VertSkew ( CPxl *pSrc, 
                    SIZE sSrc,
                    CPxl *pDst, 
                    SIZE sDst,
                    UINT uCol,
                    int iOffset,
                    double dWeight,
                    COLORREF clrBack);

    CPxl * Rotate90  (CPxl *pSrc, SIZE sSrc, SIZE *psDst);
    CPxl * Rotate180 (CPxl *pSrc, SIZE sSrc, SIZE *psDst);
    CPxl * Rotate270 (CPxl *pSrc, SIZE sSrc, SIZE *psDst);

    CPxl * Rotate45 (
        CPxl *pSrc, 
        SIZE sSrc,
        SIZE *psDst,
        double dAngle,
        COLORREF clrBack,
        BOOL bMidImage);
};  // CRotateByShear


#ifdef ROTATE_PI
#undef ROTATE_PI
#endif // ROTATE_PI

#define ROTATE_PI  double (3.1415926535897932384626433832795)


template <class CPxl>
void 
CRotateByShear<CPxl>::HorizSkew (
    CPxl *pSrc, 
    SIZE sSrc,
    CPxl *pDst, 
    SIZE sDst,
    UINT uRow, 
    int iOffset, 
    double dWeight,
    COLORREF clrBack)
/*------------------------------------------------------------------------------

  Function: HorizSkew

  Purpose:  Skews a row horizontally (with filtered weights)

  Input:    Image to skew (+ dimensions)
            Destination of skewed image (+ dimensions)
            Row index
            Skew offset
            Relative weight of right pixel
            Background color

  Output:   None.

  Remarks:  Limited to 45 degree skewing only. Filters two adjacent pixels.

------------------------------------------------------------------------------*/
{
    for (int i = 0; i < iOffset; i++)
    {
        // Fill gap left of skew with background
        SetRGB (pDst, sDst, i, uRow, clrBack);
    }

    COLORREF pxlOldLeft = clrBack;
    for (i = 0; i < sSrc.cx; i++) 
    {
        // Loop through row pixels
        COLORREF pxlSrc = GetRGB(pSrc, sSrc, i, uRow);
        // Calculate weights
        COLORREF pxlLeft = RGB (BYTE ( double (GetRValue (pxlSrc)) * dWeight ),
                                BYTE ( double (GetGValue (pxlSrc)) * dWeight ),
                                BYTE ( double (GetBValue (pxlSrc)) * dWeight ));
        // Update left over on source
        pxlSrc = RGB ( GetRValue (pxlSrc) - ( GetRValue (pxlLeft) - GetRValue (pxlOldLeft) ),
                       GetGValue (pxlSrc) - ( GetGValue (pxlLeft) - GetGValue (pxlOldLeft) ),
                       GetBValue (pxlSrc) - ( GetBValue (pxlLeft) - GetBValue (pxlOldLeft) ));
        // Check boundries 
        if ((i + iOffset >= 0) && (i + iOffset < sDst.cx))
        {
            SetRGB (pDst, sDst, i + iOffset, uRow, pxlSrc);
        }
        // Save leftover for next pixel in scan
        pxlOldLeft = pxlLeft;
    }
    // Go to rightmost point of skew
    i += iOffset;  
    if (i < sDst.cx)
    {
        // If still in image bounds, put leftovers there
        SetRGB (pDst, sDst, i, uRow, pxlOldLeft);
    }
    while (++i < sDst.cx)
    {   // Clear to the right of the skewed line with background
        SetRGB (pDst, sDst, i, uRow, clrBack);
    }
}   // CRotateByShear::HorizSkew


template <class CPxl>
void 
CRotateByShear<CPxl>::VertSkew (
    CPxl *pSrc, 
    SIZE sSrc,
    CPxl *pDst, 
    SIZE sDst,
    UINT uCol,
    int iOffset,
    double dWeight,
    COLORREF clrBack)
/*------------------------------------------------------------------------------

  Function: VertSkew

  Purpose:  Skews a column vertically (with filtered weights)

  Input:    Image to skew (+dimensions)
            Destination of skewed image (+dimensions)
            Column index
            Skew offset
            Relative weight of upper pixel
            Background color

  Output:   None.

  Remarks:  Limited to 45 degree skewing only. Filters two adjacent pixels.

------------------------------------------------------------------------------*/
{
    for (int i = 0; i < iOffset; i++)
    {
        // Fill gap above skew with background
        SetRGB (pDst, sDst, uCol, i, clrBack);
    }

    COLORREF pxlOldLeft = clrBack;

    int iYPos;
    for (i = 0; i < sSrc.cy; i++) 
    {
        // Loop through column pixels
        COLORREF pxlSrc = GetRGB (pSrc, sSrc, uCol, i);
        iYPos = i + iOffset;
        // Calculate weights
        COLORREF pxlLeft = RGB (BYTE ( double (GetRValue (pxlSrc)) * dWeight ),
                                BYTE ( double (GetGValue (pxlSrc)) * dWeight ),
                                BYTE ( double (GetBValue (pxlSrc)) * dWeight ));
        // Update left over on source
        pxlSrc = RGB ( GetRValue (pxlSrc) - ( GetRValue (pxlLeft) - GetRValue (pxlOldLeft) ),
                       GetGValue (pxlSrc) - ( GetGValue (pxlLeft) - GetGValue (pxlOldLeft) ),
                       GetBValue (pxlSrc) - ( GetBValue (pxlLeft) - GetBValue (pxlOldLeft) ));
        // Check boundries
        if ((iYPos >= 0) && (iYPos < sDst.cy))
        {
            SetRGB (pDst, sDst, uCol, iYPos, pxlSrc);
        }
        // Save leftover for next pixel in scan
        pxlOldLeft = pxlLeft;
    }
    // Go to bottom point of skew
    i = iYPos;  
    if (i < sDst.cy)
    {
        // If still in image bounds, put leftovers there
        SetRGB (pDst, sDst, uCol, i, pxlOldLeft);
    }
    while (++i < sDst.cy)
    {
        // Clear below skewed line with background
        SetRGB (pDst, sDst, uCol, i, clrBack);
    }
}   // CRotateByShear::VertSkew


template <class CPxl>
CPxl * 
CRotateByShear<CPxl>::Rotate90  (CPxl *pSrc, SIZE sSrc, SIZE *psDst)
/*------------------------------------------------------------------------------

  Function: Rotate90

  Purpose:  Rotates an image by 90 degrees (counter clockwise)

  Input:    Image to rotate (+dimensions)
            Pointer to destination size

  Output:   Pointer to newly allocated rotated image

  Remarks:  Precise rotation, no filters required.

------------------------------------------------------------------------------*/
{
    (*psDst).cx = sSrc.cy;
    (*psDst).cy = sSrc.cx;

    CPxl *pDst = CreateNewBitmap (*psDst);
    if (NULL == pDst)
    {
        return NULL;
    }
    for (UINT uY = 0; uY < UINT(sSrc.cy); uY++)
    {
        for (UINT uX = 0; uX < UINT(sSrc.cx); uX++)
        {
            SetRGB (pDst, *psDst, uY, (*psDst).cy - uX - 1, GetRGB (pSrc, sSrc, uX, uY));
        }
        if (m_Callback)
        {
            // Report progress
            if (!m_Callback (BYTE(double (uY) / double(sSrc.cy) * double(50.0))))
            {
                // Operation cancelled
                DestroyBitmap (pDst, *psDst);
                return NULL;
            }
        }
    }
    return pDst;
}   // CRotateByShear::Rotate90

template <class CPxl>
CPxl * 
CRotateByShear<CPxl>::Rotate180  (CPxl *pSrc, SIZE sSrc, SIZE *psDst)
/*------------------------------------------------------------------------------

  Function: Rotate180

  Purpose:  Rotates an image by 180 degrees (counter clockwise)

  Input:    Image to rotate (+dimensions)
            Pointer to destination size

  Output:   Pointer to newly allocated rotated image

  Remarks:  Precise rotation, no filters required.

------------------------------------------------------------------------------*/
{
    *psDst = sSrc;

    CPxl *pDst = CreateNewBitmap (*psDst);
    if (NULL == pDst)
    {
        return NULL;
    }

    for (UINT uY = 0; uY < UINT(sSrc.cy); uY++)
    {
        for (UINT uX = 0; uX < UINT(sSrc.cx); uX++)
        {
            SetRGB (pDst, *psDst, (*psDst).cx - uX - 1, (*psDst).cy - uY - 1, GetRGB (pSrc, sSrc, uX, uY));
        }
        if (m_Callback)
        {
            // Report progress
            if (!m_Callback (BYTE(double (uY) / double(sSrc.cy) * double(50.0))))
            {
                // Operation cancelled
                DestroyBitmap (pDst, *psDst);
                return NULL;
            }
        }
    }
    return pDst;
}   // CRotateByShear::Rotate180

template <class CPxl>
CPxl * 
CRotateByShear<CPxl>::Rotate270  (CPxl *pSrc, SIZE sSrc, SIZE *psDst)
/*------------------------------------------------------------------------------

  Function: Rotate270

  Purpose:  Rotates an image by 270 degrees (counter clockwise)

  Input:    Image to rotate (+dimensions)
            Pointer to destination size

  Output:   Pointer to newly allocated rotated image

  Remarks:  Precise rotation, no filters required.

------------------------------------------------------------------------------*/
{
    (*psDst).cx = sSrc.cy;
    (*psDst).cy = sSrc.cx;

    CPxl *pDst = CreateNewBitmap (*psDst);
    if (NULL == pDst)
    {
        return NULL;
    }

    for (UINT uY = 0; uY < UINT(sSrc.cy); uY++)
    {
        for (UINT uX = 0; uX < UINT(sSrc.cx); uX++)
        {
            SetRGB (pDst, *psDst, (*psDst).cx - uY - 1, uX, GetRGB (pSrc, sSrc, uX, uY));
        }
        if (m_Callback)
        {
            // Report progress
            if (!m_Callback (BYTE(double (uY) / double(sSrc.cy) * double(50.0))))
            {
                // Operation cancelled
                DestroyBitmap (pDst, *psDst);
                return NULL;
            }
        }
    }
    return pDst;
}   // CRotateByShear::Rotate270

template <class CPxl>
CPxl * 
CRotateByShear<CPxl>::Rotate45  (
    CPxl *pSrc, 
    SIZE sSrc,
    SIZE *psDst,
    double dAngle,
    COLORREF clrBack,
    BOOL bMidImage)
/*------------------------------------------------------------------------------

  Function: Rotate45

  Purpose:  Rotates an image by a given degree in range [-45.0 .. +45.0]
            (counter clockwise)

  Input:    Image to rotate (+dimensions)
            Pointer to destination size
            Degree of rotation
            Background color
            Was middle image used (for correct progress report)

  Output:   Pointer to newly allocated rotated image

  Remarks:  Using the 3-shear technique.

------------------------------------------------------------------------------*/
{
    if (0.0 == dAngle)
    {
        // No rotation at all
        (*psDst) = sSrc;
        return CopyBitmap (pSrc, sSrc);
    }

    double dRadAngle = dAngle * ROTATE_PI / double(180); // Angle in radians
    double dSinE = sin (dRadAngle);
    double dTan = tan (dRadAngle / 2.0);

    // Calc first shear (horizontal) destination image dimensions 
    SIZE sDst1;
    sDst1.cx = sSrc.cx + int(double(sSrc.cy) * fabs(dTan));
    sDst1.cy = sSrc.cy;


    /******* Perform 1st shear (horizontal) ******/

    // Allocate image for 1st shear
    CPxl *pDst1 = CreateNewBitmap (sDst1);
    if (NULL == pDst1)
    {
        return NULL;
    }
    for (UINT u = 0; u < UINT(sDst1.cy); u++) 
    {  
        double dShear;

        if (dTan >= 0.0)
        {
            // Positive angle
            dShear = (double(u) + 0.5) * dTan;
        }
        else
        {
            // Negative angle
            dShear = (double (int(u) - sDst1.cy) + 0.5) * dTan;
        }
        int iShear = int (floor (dShear));
        HorizSkew ( pSrc, 
                    sSrc, 
                    pDst1, 
                    sDst1,
                    u, 
                    iShear, 
                    dShear - double(iShear),
                    clrBack);
        if (m_Callback)
        {
            // Report progress
            BYTE bProgress = bMidImage ? 50 + BYTE(double (u) / double(sDst1.cy) * double(50.0 / 3.0)) :
                                         BYTE(double (u) / double(sDst1.cy) * double(33.33333));
            if (!m_Callback (bProgress))
            {
                // Operation cancelled
                DestroyBitmap (pDst1, sDst1);
                return NULL;
            }
        }
    }

    /******* Perform 2nd shear  (vertical) ******/

    // Calc 2nd shear (vertical) destination image dimensions
    SIZE sDst2;
    sDst2.cx = sDst1.cx;
    sDst2.cy = UINT (double (sSrc.cx) * fabs (dSinE) + double (sSrc.cy) * cos (dRadAngle)) + 1;
    // Allocate image for 2nd shear
    CPxl *pDst2 = CreateNewBitmap (sDst2);
    if (NULL == pDst2)
    {
        DestroyBitmap (pDst1, sDst1);
        return NULL;
    }

    double dOffset;     // Variable skew offset
    if (dSinE > 0.0)
    {   
        // Positive angle
        dOffset = double (sSrc.cx - 1) * dSinE;
    }
    else
    {
        // Negative angle
        dOffset = -dSinE * double (sSrc.cx - sDst2.cx);
    }

    for (u = 0; u < UINT(sDst2.cx); u++, dOffset -= dSinE) 
    {
        int iShear = int (floor (dOffset));
        VertSkew (  pDst1, 
                    sDst1,
                    pDst2,
                    sDst2,
                    u,
                    iShear,
                    dOffset - double(iShear),
                    clrBack);
        if (m_Callback)
        {
            // Report progress
            BYTE bProgress = bMidImage ? 66 + BYTE(double (u) / double(sDst2.cy) * double(50.0 / 3.0)) :
                                         33 + BYTE(double (u) / double(sDst2.cy) * double(33.33333));
            if (!m_Callback (bProgress))
            {
                // Operation cancelled
                DestroyBitmap (pDst1, sDst1);
                DestroyBitmap (pDst2, sDst2);
                return NULL;
            }
        }
    }

    /******* Perform 3rd shear (horizontal) ******/

    // Free result of 1st shear
    DestroyBitmap (pDst1, sDst1);

    // Calc 3rd shear (horizontal) destination image dimensions
    (*psDst).cx = UINT (double(sSrc.cy) * fabs (dSinE) + double(sSrc.cx) * cos (dRadAngle)) + 1;
    (*psDst).cy = sDst2.cy;

    // Allocate image for 3rd shear
    CPxl *pDst3 = CreateNewBitmap (*psDst);

    if (NULL == pDst3)
    {
        DestroyBitmap (pDst2, sDst2);
        return NULL;
    }

    if (dSinE >= 0.0)
    {
        // Positive angle
        dOffset = double(sSrc.cx - 1) * dSinE * -dTan;
    }
    else 
    {
        // Negative angle
        dOffset = dTan * (double(sSrc.cx - 1) * -dSinE + double(1 - (*psDst).cy));
    }
    for (u = 0; u < UINT((*psDst).cy); u++, dOffset += dTan)
    {
        int iShear = int (floor(dOffset));
        HorizSkew ( pDst2,
                    sDst2,
                    pDst3,
                    (*psDst),
                    u,
                    iShear,
                    dOffset - double (iShear),
                    clrBack
                  );
        if (m_Callback)
        {
            // Report progress
            BYTE bProgress = bMidImage ? 83 + BYTE(double (u) / double((*psDst).cy) * double(50.0 / 3.0)) :
                                         66 + BYTE(double (u) / double((*psDst).cy) * double(33.33333));
            if (!m_Callback (bProgress))
            {
                // Operation cancelled
                DestroyBitmap (pDst2, sDst2);
                DestroyBitmap (pDst3, *psDst);
                return NULL;
            }
        }
    }
    // Free result of 2nd shear    
    DestroyBitmap (pDst2, sDst2);
    // Return result of 3rd shear
    return pDst3;      
}   // CRotateByShear::Rotate45


template <class CPxl>
CPxl * 
CRotateByShear<CPxl>::AllocAndRotate (  
    CPxl       *pSrc, 
    SIZE        sSrc,
    double      dAngle,
    SIZE       *psDst,
    COLORREF    clrBack
)
/*------------------------------------------------------------------------------

  Function: AllocAndRotate

  Purpose:  Rotates an image by a given degree

  Input:    Image to rotate (+dimensions)
            Angle of rotation
            Pointers to dimensions of rotated image
            Background color

  Output:   Pointer to newly allocated rotated image

  Remarks:  Angle is unlimited. 3-shears technique is used.

------------------------------------------------------------------------------*/
{
    CPxl *pMidImg = pSrc;
    SIZE sMidImg = sSrc;

    if (NULL == pSrc)
    {
        return NULL;
    }

    while (dAngle >= 360.0)
    {
        // Bring angle to range of (-INF .. 360.0)
        dAngle -= 360.0;
    }
    while (dAngle < 0.0)
    {
        // Bring angle to range of [0.0 .. 360.0) 
        dAngle += 360.0;
    }
    if ((dAngle > 45.0) && (dAngle <= 135.0)) 
    {
        // Angle in (45.0 .. 135.0] 
        // Rotate image by 90 degrees into temporary image,
        // so it requires only an extra rotation angle 
        // of -45.0 .. +45.0 to complete rotation.
        pMidImg = Rotate90 (pSrc, sSrc, &sMidImg);
        dAngle -= 90.0;
    }
    else if ((dAngle > 135.0) && (dAngle <= 225.0)) 
    { 
        // Angle in (135.0 .. 225.0] 
        // Rotate image by 180 degrees into temporary image,
        // so it requires only an extra rotation angle 
        // of -45.0 .. +45.0 to complete rotation.
        pMidImg = Rotate180 (pSrc, sSrc, &sMidImg);
        dAngle -= 180.0;
    }
    else if ((dAngle > 225.0) && (dAngle <= 315.0)) 
    { 
        // Angle in (225.0 .. 315.0] 
        // Rotate image by 270 degrees into temporary image,
        // so it requires only an extra rotation angle 
        // of -45.0 .. +45.0 to complete rotation.
        pMidImg = Rotate270 (pSrc, sSrc, &sMidImg);
        dAngle -= 270.0;
    }

    // If we got here, angle is in (-45.0 .. +45.0]

    if (NULL == pMidImg)
    {
        // Failed to allocate middle image
        return NULL;
    }

    CPxl *pDst = Rotate45 ( pMidImg,
                            sMidImg,
                            psDst,
                            dAngle,
                            clrBack,
                            pSrc != pMidImg);

    if (pSrc != pMidImg)
    {
        // Middle image was required, free it now.
        DestroyBitmap (pMidImg, sMidImg);
    }
    return pDst;
}   // AllocAndRotate



#endif

