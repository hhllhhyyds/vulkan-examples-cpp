#include <vector>
#include <fstream>
#include <string>

#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/Include/ResourceLimits.h>

#include "shader.h"

bool ShaderCompileEnv::initialized = false;

std::vector<char> read_file(const std::string &file_path)
{
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        std::cerr << "Failed to open " << file_path << std::endl;
        throw std::runtime_error("Failed to open a file!");
    }

    size_t file_size = file.tellg();

    std::vector<char> file_buffer(file_size + 1, '\0');

    file.seekg(0);
    file.read(file_buffer.data(), file_size);

    file.close();

    return file_buffer;
}

void init_resource(TBuiltInResource &resource)
{
    resource.maxLights = 32;
    resource.maxClipPlanes = 6;
    resource.maxTextureUnits = 32;
    resource.maxTextureCoords = 32;
    resource.maxVertexAttribs = 64;
    resource.maxVertexUniformComponents = 4096;
    resource.maxVaryingFloats = 64;
    resource.maxVertexTextureImageUnits = 32;
    resource.maxCombinedTextureImageUnits = 80;
    resource.maxTextureImageUnits = 32;
    resource.maxFragmentUniformComponents = 4096;
    resource.maxDrawBuffers = 32;
    resource.maxVertexUniformVectors = 128;
    resource.maxVaryingVectors = 8;
    resource.maxFragmentUniformVectors = 16;
    resource.maxVertexOutputVectors = 16;
    resource.maxFragmentInputVectors = 15;
    resource.minProgramTexelOffset = -8;
    resource.maxProgramTexelOffset = 7;
    resource.maxClipDistances = 8;
    resource.maxComputeWorkGroupCountX = 65535;
    resource.maxComputeWorkGroupCountY = 65535;
    resource.maxComputeWorkGroupCountZ = 65535;
    resource.maxComputeWorkGroupSizeX = 1024;
    resource.maxComputeWorkGroupSizeY = 1024;
    resource.maxComputeWorkGroupSizeZ = 64;
    resource.maxComputeUniformComponents = 1024;
    resource.maxComputeTextureImageUnits = 16;
    resource.maxComputeImageUniforms = 8;
    resource.maxComputeAtomicCounters = 8;
    resource.maxComputeAtomicCounterBuffers = 1;
    resource.maxVaryingComponents = 60;
    resource.maxVertexOutputComponents = 64;
    resource.maxGeometryInputComponents = 64;
    resource.maxGeometryOutputComponents = 128;
    resource.maxFragmentInputComponents = 128;
    resource.maxImageUnits = 8;
    resource.maxCombinedImageUnitsAndFragmentOutputs = 8;
    resource.maxCombinedShaderOutputResources = 8;
    resource.maxImageSamples = 0;
    resource.maxVertexImageUniforms = 0;
    resource.maxTessControlImageUniforms = 0;
    resource.maxTessEvaluationImageUniforms = 0;
    resource.maxGeometryImageUniforms = 0;
    resource.maxFragmentImageUniforms = 8;
    resource.maxCombinedImageUniforms = 8;
    resource.maxGeometryTextureImageUnits = 16;
    resource.maxGeometryOutputVertices = 256;
    resource.maxGeometryTotalOutputComponents = 1024;
    resource.maxGeometryUniformComponents = 1024;
    resource.maxGeometryVaryingComponents = 64;
    resource.maxTessControlInputComponents = 128;
    resource.maxTessControlOutputComponents = 128;
    resource.maxTessControlTextureImageUnits = 16;
    resource.maxTessControlUniformComponents = 1024;
    resource.maxTessControlTotalOutputComponents = 4096;
    resource.maxTessEvaluationInputComponents = 128;
    resource.maxTessEvaluationOutputComponents = 128;
    resource.maxTessEvaluationTextureImageUnits = 16;
    resource.maxTessEvaluationUniformComponents = 1024;
    resource.maxTessPatchComponents = 120;
    resource.maxPatchVertices = 32;
    resource.maxTessGenLevel = 64;
    resource.maxViewports = 16;
    resource.maxVertexAtomicCounters = 0;
    resource.maxTessControlAtomicCounters = 0;
    resource.maxTessEvaluationAtomicCounters = 0;
    resource.maxGeometryAtomicCounters = 0;
    resource.maxFragmentAtomicCounters = 8;
    resource.maxCombinedAtomicCounters = 8;
    resource.maxAtomicCounterBindings = 1;
    resource.maxVertexAtomicCounterBuffers = 0;
    resource.maxTessControlAtomicCounterBuffers = 0;
    resource.maxTessEvaluationAtomicCounterBuffers = 0;
    resource.maxGeometryAtomicCounterBuffers = 0;
    resource.maxFragmentAtomicCounterBuffers = 1;
    resource.maxCombinedAtomicCounterBuffers = 1;
    resource.maxAtomicCounterBufferSize = 16384;
    resource.maxTransformFeedbackBuffers = 4;
    resource.maxTransformFeedbackInterleavedComponents = 64;
    resource.maxCullDistances = 8;
    resource.maxCombinedClipAndCullDistances = 8;
    resource.maxSamples = 4;
    resource.maxMeshOutputVerticesNV = 256;
    resource.maxMeshOutputPrimitivesNV = 512;
    resource.maxMeshWorkGroupSizeX_NV = 32;
    resource.maxMeshWorkGroupSizeY_NV = 1;
    resource.maxMeshWorkGroupSizeZ_NV = 1;
    resource.maxTaskWorkGroupSizeX_NV = 32;
    resource.maxTaskWorkGroupSizeY_NV = 1;
    resource.maxTaskWorkGroupSizeZ_NV = 1;
    resource.maxMeshViewCountNV = 4;
    resource.limits.nonInductiveForLoops = 1;
    resource.limits.whileLoops = 1;
    resource.limits.doWhileLoops = 1;
    resource.limits.generalUniformIndexing = 1;
    resource.limits.generalAttributeMatrixVectorIndexing = 1;
    resource.limits.generalVaryingIndexing = 1;
    resource.limits.generalSamplerIndexing = 1;
    resource.limits.generalVariableIndexing = 1;
    resource.limits.generalConstantMatrixVectorIndexing = 1;
}

EShLanguage find_language(const VkShaderStageFlagBits shader_type)
{
    switch (shader_type)
    {
    case VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT:
        return EShLangVertex;
    case VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
        return EShLangTessControl;
    case VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
        return EShLangTessEvaluation;
    case VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT:
        return EShLangGeometry;
    case VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT:
        return EShLangFragment;
    case VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT:
        return EShLangCompute;
    default:
        return EShLangVertex;
    }
}

ShaderModule::ShaderModule(std::shared_ptr<LogicalDevice> device, const VkShaderStageFlagBits shader_type, const std::string &file_path)
    : ShaderModule(device, shader_type, read_file(file_path)) {}

ShaderModule::ShaderModule(std::shared_ptr<LogicalDevice> device, const VkShaderStageFlagBits shader_type, const std::vector<char> &shader_source)
    : device(device), shader_type(shader_type)
{
    if (!ShaderCompileEnv::is_initialized())
    {
        throw std::runtime_error("Shader compile env is not initialized!");
    }

    auto lang = find_language(shader_type);
    glslang::TShader shader(lang);

    const char *shader_strings[1];
    shader_strings[0] = shader_source.data();
    shader.setStrings(shader_strings, 1);

    shader.setEnvInput(glslang::EShSourceGlsl, lang, glslang::EShClientVulkan, 100);
    shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_1);
    shader.setEnvTarget(glslang::EshTargetSpv, glslang::EShTargetSpv_1_3);

    TBuiltInResource resource{};
    if (!shader.parse(&resource, 100, ENoProfile, false, false, EShMsgDefault))
    {
        std::cerr << shader.getInfoLog();
        return;
    }

    glslang::TProgram program;
    program.addShader(&shader);
    if (!program.link(EShMsgDefault))
    {
        std::cerr << program.getInfoLog();
        return;
    }
    const auto intermediate = program.getIntermediate(lang);

    glslang::GlslangToSpv(*intermediate, spirv);

    VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = spirv.size() * sizeof(uint32_t);
    shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(spirv.data());

    auto result = vkCreateShaderModule(device->get_vk_device(), &shaderModuleCreateInfo, nullptr, &module);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shader module!");
    }

    is_valid = true;
}

ShaderModule::~ShaderModule()
{
    vkDestroyShaderModule(device->get_vk_device(), module, VK_NULL_HANDLE);
    module = VK_NULL_HANDLE;
    is_valid = false;
}