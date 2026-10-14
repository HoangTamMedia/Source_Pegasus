
#include "stdafx.h"

#if defined LDK_ADD_SCALEFORM && defined GFX_UI_TEST_CODE

#include <assert.h>
#include "GSysFile.h"

#include "ZzzTexture.h"
#include "CGFxSample.h"

GFile* Fx3DDemoFileOpener::OpenFile(const char *pfilename)
{
	GFile * temp = new GSysFile(pfilename);

	return temp;
}

GImageInfoBase* CustomImageCreator::CreateImage(const GFxImageCreateInfo &info)
{
	GPtr<GImage> pimage;

	switch(info.Type)
	{
	case GFxImageCreateInfo::Input_Image:

		return new GImageInfo(info.pImage, info.pImage->Width, info.pImage->Height);

	case GFxImageCreateInfo::Input_File:
		{

  			GPtr<GTextureOGL>texture = pRenderer->CreateTexture();
  			texture->InitTexture(  Bitmaps.GetTexture(BITMAP_TEST_UI_IMAGE)->BitmapIndex );
  			GImageInfo * imageInfo = new GImageInfo( (GTexture*)texture, info.pFileInfo->TargetWidth, info.pFileInfo->TargetHeight );
			if(!imageInfo)
				return 0;

			return imageInfo;
