// ------------------------------------------------------------
// Copyright(c) 2018-2022 Jesse Yurkovich
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// See the LICENSE file in the repo root for full license information.
// ------------------------------------------------------------
#pragma once

#include <fx/gltf.h>

#if defined(_MSC_VER)
    #if _MSC_VER < 1914 || (!defined(_HAS_CXX17) || (defined(_HAS_CXX17) && _HAS_CXX17 == 0))
        #define VIEWER_EXPERIMENTAL_FILESYSTEM
    #endif
#endif

#ifdef VIEWER_EXPERIMENTAL_FILESYSTEM
    #include <experimental/filesystem>
    #define VIEWER_FILESYSTEM std::experimental::filesystem::v1
#else
    #include <filesystem>
    #define VIEWER_FILESYSTEM std::filesystem
#endif

class ImageData
{
public:
    struct ImageInfo
    {
        VIEWER_FILESYSTEM::path FileName{};

        uint32_t BinarySize{};
        uint8_t const * BinaryData{};

        bool IsBinary() const noexcept
        {
            return BinaryData != nullptr;
        }
    };

    explicit ImageData(std::string const & texture)
    {
        m_info.FileName = texture;
    }

    ImageData(fx::gltf::Document const & doc, std::size_t textureIndex, std::string const & modelPath)
    {
        fx::gltf::Image const & image = doc.images[doc.textures[textureIndex].source];

        const bool isEmbedded = image.IsEmbeddedResource();
        if (!image.uri.empty() && !isEmbedded)
        {
            m_info.FileName = fx::gltf::detail::GetDocumentRootPath(modelPath) / image.uri;
        }
        else
        {
            if (isEmbedded)
            {
                image.MaterializeData(m_embeddedData);
                m_info.BinaryData = &m_embeddedData[0];
                m_info.BinarySize = static_cast<uint32_t>(m_embeddedData.size());
            }
            else
            {
                fx::gltf::BufferView const & bufferView = doc.bufferViews[image.bufferView];
                fx::gltf::Buffer const & buffer = doc.buffers[bufferView.buffer];

                m_info.BinaryData = &buffer.data[bufferView.byteOffset];
                m_info.BinarySize = bufferView.byteLength;
            }
        }
    }

    ImageInfo const & Info() const noexcept
    {
        return m_info;
    }

private:
    ImageInfo m_info{};

    std::vector<uint8_t> m_embeddedData{};
};
