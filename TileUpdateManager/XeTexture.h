//*********************************************************
//
// Copyright 2020 Intel Corporation 
//
// Permission is hereby granted, free of charge, to any 
// person obtaining a copy of this software and associated 
// documentation files(the "Software"), to deal in the Software 
// without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to 
// whom the Software is furnished to do so, subject to the 
// following conditions :
// The above copyright notice and this permission notice shall 
// be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.
//
//*********************************************************

/*=============================================================================
Object that knows how to parse find tile offsets in an XeT texture
Holds the file handle
=============================================================================*/

#pragma once

#include <d3d12.h>
#include <vector>
#include "Timer.h"

#include "XetFileHeader.h"

namespace Streaming
{
    class XeTexture
    {
    public:
        DXGI_FORMAT GetFormat() const { return m_fileHeader.m_extensionHeader.dxgiFormat; }
        UINT GetImageWidth() const { return m_fileHeader.m_ddsHeader.width; }
        UINT GetImageHeight() const { return m_fileHeader.m_ddsHeader.height; }
        UINT GetMipCount() const { return m_fileHeader.m_ddsHeader.mipMapCount; }

        const BYTE* GetPackedBits(UINT& out_numBytes) const { out_numBytes = (UINT)m_packedMips.size(); return m_packedMips.data(); }

        // WritePackedBits() is for the packed portion of the mip map, populated at startup.
        // The pointer will be to a footprint for copying, with destination stride likely larger than mip contents.
        void WritePackedBits(void* out_pBits, UINT in_mip, UINT64 in_dstStrideBytes);

        // return value is # bytes. out_offset is byte offset into file
        UINT GetFileOffset(const D3D12_TILED_RESOURCE_COORDINATE& in_coord) const;

        XeTexture(const std::wstring& in_filename);
    protected:
        XeTexture(const XeTexture&) = delete;
        XeTexture(XeTexture&&) = delete;
        XeTexture& operator=(const XeTexture&) = delete;
        XeTexture& operator=(XeTexture&&) = delete;

        static const UINT MIN_STRIDE_BYTES{ 256 };
        static const UINT NUM_BYTES_PER_TILE{ D3D12_TILED_RESOURCE_TILE_SIZE_IN_BYTES }; // tiles are always 64KB in size

        Timer m_cpuTimer;

        XetFileHeader m_fileHeader;

        std::vector<XetFileHeader::TileData> m_tileOffsets;
        std::vector<XetFileHeader::MetaData> m_metadataOffsets;

        // bytes for packed mip are read at creation time
        std::vector<BYTE> m_packedMips;

        UINT GetLinearIndex(const D3D12_TILED_RESOURCE_COORDINATE& in_coord) const;
    };
}
