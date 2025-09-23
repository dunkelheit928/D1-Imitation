#include "pch.h"
#include "Function.h"

bool LoadImageRGBA(const wchar_t* path, std::vector<BYTE>& outPixel, int& outWidth, int& outHeight)
{
	CoInitialize(nullptr);

	ComPtr<IWICImagingFactory> factory;
	CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));

	ComPtr<IWICBitmapDecoder> decoder;
	factory->CreateDecoderFromFilename(path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);

	ComPtr<IWICBitmapFrameDecode> frame;
	decoder->GetFrame(0, &frame);

	ComPtr<IWICFormatConverter> conv;
	factory->CreateFormatConverter(&conv);
	conv->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

	UINT w, h;
	conv->GetSize(&w, &h);
	outWidth = (int)w;
	outHeight = (int)h;
	outPixel.resize(outWidth * outHeight * 4);
	conv->CopyPixels(nullptr, w * 4, (UINT)outPixel.size(), outPixel.data());

	return true;
}