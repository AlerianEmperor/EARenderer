//
//  DiffuseLightProbeData.cpp
//  EARenderer
//
//  Created by Pavlo Muratov on 28.09.2018.
//  Copyright © 2018 MPO. All rights reserved.
//

#include "DiffuseLightProbeData.hpp"
#include "StringUtils.hpp"
#include "Serializers.hpp"

#include <bitsery/bitsery.h>
#include <bitsery/traits/vector.h>
#include <bitsery/adapter/stream.h>
#include <fstream>

namespace EARenderer {

#pragma mark - Data

    void DiffuseLightProbeData::initializeBuffers() {
        // Transfer spherical harmonics coefficients to the GPU via buffer texture
        std::vector<SphericalHarmonics> shs;
        for (auto &projection : mSurfelClusterProjections) {
            shs.push_back(projection.sphericalHarmonics);
        }

        // Transfer surfel cluster indices to the GPU via buffer texture
        std::vector<uint32_t> indices;
        for (auto &projection : mSurfelClusterProjections) {
            indices.push_back(static_cast<uint32_t>(projection.surfelClusterIndex));
        }

        // Transfer surfel cluster projection group offsets, sizes and probe positions to the GPU via buffer texture
        std::vector<SphericalHarmonics> skySHs;
        std::vector<uint32_t> metadata;
        std::vector<glm::vec3> positions;

        for (auto &probe : mProbes) {
            metadata.push_back((uint32_t) probe.surfelClusterProjectionGroupOffset);
            metadata.push_back((uint32_t) probe.surfelClusterProjectionGroupSize);
            positions.push_back(probe.position);
            skySHs.push_back(probe.skySphericalHarmonics);
        }

        mProjectionClusterSHsBufferTexture = std::make_shared<GLFloatBufferTexture<GLTexture::Float::RGB32F, SphericalHarmonics>>(shs.data(), shs.size());
        mSkySHsBufferTexture = std::make_shared<GLFloatBufferTexture<GLTexture::Float::RGB32F, SphericalHarmonics>>(skySHs.data(), skySHs.size());
        mProjectionClusterIndicesBufferTexture = std::make_shared<GLIntegerBufferTexture<GLTexture::Integer::R32UI, uint32_t>>(indices.data(), indices.size());
        mProbeClusterProjectionsMetadataBufferTexture = std::make_shared<GLIntegerBufferTexture<GLTexture::Integer::R32UI, uint32_t>>(metadata.data(), metadata.size());
        mProbePositionsBufferTexture = std::make_shared<GLFloatBufferTexture<GLTexture::Float::RGB32F, glm::vec3>>(positions.data(), positions.size());
    }

    void DiffuseLightProbeData::serialize(const std::string &filePath) {
        std::ofstream stream(filePath, std::ios::trunc | std::ios::binary);
        if (!stream.is_open()) {
            throw std::runtime_error(string_format("Unable to serialize light probes: %s", filePath.c_str()));
        }

        bitsery::Serializer<bitsery::OutputBufferedStreamAdapter> serializer(stream);
        serializer.container(mProbes, mProbes.size());
        serializer.container(mSurfelClusterProjections, mSurfelClusterProjections.size());
        serializer.object(mGridResolution);
        bitsery::AdapterAccess::getWriter(serializer).flush();
    }

    bool DiffuseLightProbeData::deserialize(const std::string &filePath) {
        std::ifstream stream(filePath);
        if (!stream.is_open()) {
            return false;
        }

        bitsery::Deserializer<bitsery::InputStreamAdapter> deserializer(stream);
        deserializer.container(mProbes, std::numeric_limits<uint32_t>::max());
        deserializer.container(mSurfelClusterProjections, std::numeric_limits<uint32_t>::max());
        deserializer.object(mGridResolution);

        auto &reader = bitsery::AdapterAccess::getReader(deserializer);

        if (reader.isCompletedSuccessfully()) {
            initializeBuffers();
        }

        return reader.isCompletedSuccessfully();
    }

#pragma mark - Getters

    const std::vector<DiffuseLightProbe> &DiffuseLightProbeData::probes() const {
        return mProbes;
    }

    const std::vector<SurfelClusterProjection> &DiffuseLightProbeData::surfelClusterProjections() const {
        return mSurfelClusterProjections;
    }

    const glm::ivec3 &DiffuseLightProbeData::gridResolution() const {
        return mGridResolution;
    }

    std::shared_ptr<GLFloatBufferTexture<GLTexture::Float::RGB32F, SphericalHarmonics>> DiffuseLightProbeData::projectionClusterSHsBufferTexture() const {
        return mProjectionClusterSHsBufferTexture;
    }

    std::shared_ptr<GLFloatBufferTexture<GLTexture::Float::RGB32F, SphericalHarmonics>> DiffuseLightProbeData::skySHsBufferTexture() const {
        return mSkySHsBufferTexture;
    }

    std::shared_ptr<GLIntegerBufferTexture<GLTexture::Integer::R32UI, uint32_t>> DiffuseLightProbeData::projectionClusterIndicesBufferTexture() const {
        return mProjectionClusterIndicesBufferTexture;
    };

    std::shared_ptr<GLIntegerBufferTexture<GLTexture::Integer::R32UI, uint32_t>> DiffuseLightProbeData::probeClusterProjectionsMetadataBufferTexture() const {
        return mProbeClusterProjectionsMetadataBufferTexture;
    }

    std::shared_ptr<GLFloatBufferTexture<GLTexture::Float::RGB32F, glm::vec3>> DiffuseLightProbeData::probePositionsBufferTexture() const {
        return mProbePositionsBufferTexture;
    }

}
