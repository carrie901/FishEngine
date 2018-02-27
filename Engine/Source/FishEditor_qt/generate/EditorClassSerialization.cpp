
/**************************************************
* auto generated by reflection system
**************************************************/

#include <FishEngine/Serialization/Archive.hpp>
#include <FishEngine/Private/CloneUtility.hpp>
#include "../DDSImporter.hpp" 
#include "../ShaderImporter.hpp" 
#include "../TextureImporter.hpp" 
#include "../AssetImporter.hpp" 
#include "../AudioImporterSampleSettings.hpp" 
#include "../ProjectSettings.hpp" 
#include "../PropertyModification.hpp" 
#include "../NativeFormatImporter.hpp" 
#include "../PrefabUtility.hpp" 
#include "../ModelImporter.hpp" 
#include "../EditorGUILayout.hpp" 
#include "../EditorUtility.hpp" 
#include "../AudioImporter.hpp" 

namespace FishEditor
{

	// FishEditor::AssetImporter
	void FishEditor::AssetImporter::Serialize ( FishEngine::OutputArchive & archive ) const
	{
		//archive.BeginClass();
		archive << FishEngine::make_nvp("m_assetBundleName", m_assetBundleName); // std::string
		archive << FishEngine::make_nvp("m_assetBundelVariant", m_assetBundelVariant); // std::string
		archive << FishEngine::make_nvp("m_userData", m_userData); // std::string
		archive << FishEngine::make_nvp("m_fileIDToRecycleName", m_fileIDToRecycleName); // std::map<int, std::string>
		//archive.EndClass();
	}

	void FishEditor::AssetImporter::Deserialize ( FishEngine::InputArchive & archive )
	{
		//archive.BeginClass(2);
		archive >> FishEngine::make_nvp("m_assetBundleName", m_assetBundleName); // std::string
		archive >> FishEngine::make_nvp("m_assetBundelVariant", m_assetBundelVariant); // std::string
		archive >> FishEngine::make_nvp("m_userData", m_userData); // std::string
		archive >> FishEngine::make_nvp("m_fileIDToRecycleName", m_fileIDToRecycleName); // std::map<int, std::string>
		//archive.EndClass();
	}



	// FishEditor::ShaderImporter
	void FishEditor::ShaderImporter::Serialize ( FishEngine::OutputArchive & archive ) const
	{
		//archive.BeginClass();
		FishEditor::AssetImporter::Serialize(archive);
		//archive.EndClass();
	}

	void FishEditor::ShaderImporter::Deserialize ( FishEngine::InputArchive & archive )
	{
		//archive.BeginClass(2);
		FishEditor::AssetImporter::Deserialize(archive);
		//archive.EndClass();
	}



	// FishEditor::DDSImporter
	void FishEditor::DDSImporter::Serialize ( FishEngine::OutputArchive & archive ) const
	{
		//archive.BeginClass();
		FishEditor::AssetImporter::Serialize(archive);
		//archive.EndClass();
	}

	void FishEditor::DDSImporter::Deserialize ( FishEngine::InputArchive & archive )
	{
		//archive.BeginClass(2);
		FishEditor::AssetImporter::Deserialize(archive);
		//archive.EndClass();
	}



	// FishEditor::EditorUtility
	FishEngine::OutputArchive & operator << ( FishEngine::OutputArchive & archive, FishEditor::EditorUtility const & value )
	{
		archive.BeginClass();
		archive.EndClass();
		return archive;
	}

	FishEngine::InputArchive & operator >> ( FishEngine::InputArchive & archive, FishEditor::EditorUtility & value )
	{
		archive.BeginClass();
		archive.EndClass();
		return archive;
	}

	// FishEditor::ProjectSettings
	FishEngine::OutputArchive & operator << ( FishEngine::OutputArchive & archive, FishEditor::ProjectSettings const & value )
	{
		archive.BeginClass();
		archive.EndClass();
		return archive;
	}

	FishEngine::InputArchive & operator >> ( FishEngine::InputArchive & archive, FishEditor::ProjectSettings & value )
	{
		archive.BeginClass();
		archive.EndClass();
		return archive;
	}

	// FishEditor::EditorGUILayout
	FishEngine::OutputArchive & operator << ( FishEngine::OutputArchive & archive, FishEditor::EditorGUILayout const & value )
	{
		archive.BeginClass();
		archive.EndClass();
		return archive;
	}

	FishEngine::InputArchive & operator >> ( FishEngine::InputArchive & archive, FishEditor::EditorGUILayout & value )
	{
		archive.BeginClass();
		archive.EndClass();
		return archive;
	}

	// FishEditor::NativeFormatImporter
	void FishEditor::NativeFormatImporter::Serialize ( FishEngine::OutputArchive & archive ) const
	{
		//archive.BeginClass();
		FishEditor::AssetImporter::Serialize(archive);
		//archive.EndClass();
	}

	void FishEditor::NativeFormatImporter::Deserialize ( FishEngine::InputArchive & archive )
	{
		//archive.BeginClass(2);
		FishEditor::AssetImporter::Deserialize(archive);
		//archive.EndClass();
	}



	// FishEditor::PropertyModification
	FishEngine::OutputArchive & operator << ( FishEngine::OutputArchive & archive, FishEditor::PropertyModification const & value )
	{
		archive.BeginClass();
		archive << FishEngine::make_nvp("target", value.target); // FishEngine::ObjectPtr
		archive << FishEngine::make_nvp("propertyPath", value.propertyPath); // std::string
		archive << FishEngine::make_nvp("value", value.value); // std::string
		archive << FishEngine::make_nvp("objectReference", value.objectReference); // FishEngine::ObjectPtr
		archive.EndClass();
		return archive;
	}

	FishEngine::InputArchive & operator >> ( FishEngine::InputArchive & archive, FishEditor::PropertyModification & value )
	{
		archive.BeginClass();
		archive >> FishEngine::make_nvp("target", value.target); // FishEngine::ObjectPtr
		archive >> FishEngine::make_nvp("propertyPath", value.propertyPath); // std::string
		archive >> FishEngine::make_nvp("value", value.value); // std::string
		archive >> FishEngine::make_nvp("objectReference", value.objectReference); // FishEngine::ObjectPtr
		archive.EndClass();
		return archive;
	}

	// FishEditor::AudioImporter
	void FishEditor::AudioImporter::Serialize ( FishEngine::OutputArchive & archive ) const
	{
		//archive.BeginClass();
		FishEditor::AssetImporter::Serialize(archive);
		archive << FishEngine::make_nvp("m_defaultSampleSettings", m_defaultSampleSettings); // FishEditor::AudioImporterSampleSettings
		archive << FishEngine::make_nvp("m_forceToMono", m_forceToMono); // bool
		archive << FishEngine::make_nvp("m_loadInBackground", m_loadInBackground); // bool
		archive << FishEngine::make_nvp("m_preloadAudioData", m_preloadAudioData); // bool
		//archive.EndClass();
	}

	void FishEditor::AudioImporter::Deserialize ( FishEngine::InputArchive & archive )
	{
		//archive.BeginClass(2);
		FishEditor::AssetImporter::Deserialize(archive);
		archive >> FishEngine::make_nvp("m_defaultSampleSettings", m_defaultSampleSettings); // FishEditor::AudioImporterSampleSettings
		archive >> FishEngine::make_nvp("m_forceToMono", m_forceToMono); // bool
		archive >> FishEngine::make_nvp("m_loadInBackground", m_loadInBackground); // bool
		archive >> FishEngine::make_nvp("m_preloadAudioData", m_preloadAudioData); // bool
		//archive.EndClass();
	}



	// FishEditor::AudioImporterSampleSettings
	FishEngine::OutputArchive & operator << ( FishEngine::OutputArchive & archive, FishEditor::AudioImporterSampleSettings const & value )
	{
		archive.BeginClass();
		archive << FishEngine::make_nvp("loadType", value.loadType); // FishEngine::AudioClipLoadType
		archive << FishEngine::make_nvp("sampleRateSetting", value.sampleRateSetting); // FishEditor::AudioSampleRateSetting
		archive << FishEngine::make_nvp("sampleRateOverride", value.sampleRateOverride); // uint32_t
		archive << FishEngine::make_nvp("compressionFormat", value.compressionFormat); // FishEngine::AudioCompressionFormat
		archive << FishEngine::make_nvp("quality", value.quality); // float
		archive << FishEngine::make_nvp("conversionMode", value.conversionMode); // int
		archive.EndClass();
		return archive;
	}

	FishEngine::InputArchive & operator >> ( FishEngine::InputArchive & archive, FishEditor::AudioImporterSampleSettings & value )
	{
		archive.BeginClass();
		archive >> FishEngine::make_nvp("loadType", value.loadType); // FishEngine::AudioClipLoadType
		archive >> FishEngine::make_nvp("sampleRateSetting", value.sampleRateSetting); // FishEditor::AudioSampleRateSetting
		archive >> FishEngine::make_nvp("sampleRateOverride", value.sampleRateOverride); // uint32_t
		archive >> FishEngine::make_nvp("compressionFormat", value.compressionFormat); // FishEngine::AudioCompressionFormat
		archive >> FishEngine::make_nvp("quality", value.quality); // float
		archive >> FishEngine::make_nvp("conversionMode", value.conversionMode); // int
		archive.EndClass();
		return archive;
	}

	// FishEditor::ModelImporter
	void FishEditor::ModelImporter::Serialize ( FishEngine::OutputArchive & archive ) const
	{
		//archive.BeginClass();
		FishEditor::AssetImporter::Serialize(archive);
		archive << FishEngine::make_nvp("m_globalScale", m_globalScale); // float
		archive << FishEngine::make_nvp("m_importNormals", m_importNormals); // FishEditor::ModelImporterNormals
		archive << FishEngine::make_nvp("m_importTangents", m_importTangents); // FishEditor::ModelImporterTangents
		archive << FishEngine::make_nvp("m_materialSearch", m_materialSearch); // FishEditor::ModelImporterMaterialSearch
		//archive.EndClass();
	}

	void FishEditor::ModelImporter::Deserialize ( FishEngine::InputArchive & archive )
	{
		//archive.BeginClass(2);
		FishEditor::AssetImporter::Deserialize(archive);
		archive >> FishEngine::make_nvp("m_globalScale", m_globalScale); // float
		archive >> FishEngine::make_nvp("m_importNormals", m_importNormals); // FishEditor::ModelImporterNormals
		archive >> FishEngine::make_nvp("m_importTangents", m_importTangents); // FishEditor::ModelImporterTangents
		archive >> FishEngine::make_nvp("m_materialSearch", m_materialSearch); // FishEditor::ModelImporterMaterialSearch
		//archive.EndClass();
	}



	// FishEditor::PrefabUtility
	FishEngine::OutputArchive & operator << ( FishEngine::OutputArchive & archive, FishEditor::PrefabUtility const & value )
	{
		archive.BeginClass();
		archive.EndClass();
		return archive;
	}

	FishEngine::InputArchive & operator >> ( FishEngine::InputArchive & archive, FishEditor::PrefabUtility & value )
	{
		archive.BeginClass();
		archive.EndClass();
		return archive;
	}

	// FishEditor::TextureSettings
	FishEngine::OutputArchive & operator << ( FishEngine::OutputArchive & archive, FishEditor::TextureSettings const & value )
	{
		archive.BeginClass();
		archive << FishEngine::make_nvp("m_filterMode", value.m_filterMode); // FishEngine::FilterMode
		archive << FishEngine::make_nvp("m_aniso", value.m_aniso); // int
		archive << FishEngine::make_nvp("m_mipBias", value.m_mipBias); // float
		archive << FishEngine::make_nvp("m_wrapMode", value.m_wrapMode); // FishEngine::TextureWrapMode
		archive.EndClass();
		return archive;
	}

	FishEngine::InputArchive & operator >> ( FishEngine::InputArchive & archive, FishEditor::TextureSettings & value )
	{
		archive.BeginClass();
		archive >> FishEngine::make_nvp("m_filterMode", value.m_filterMode); // FishEngine::FilterMode
		archive >> FishEngine::make_nvp("m_aniso", value.m_aniso); // int
		archive >> FishEngine::make_nvp("m_mipBias", value.m_mipBias); // float
		archive >> FishEngine::make_nvp("m_wrapMode", value.m_wrapMode); // FishEngine::TextureWrapMode
		archive.EndClass();
		return archive;
	}

	// FishEditor::TextureImporter
	void FishEditor::TextureImporter::Serialize ( FishEngine::OutputArchive & archive ) const
	{
		//archive.BeginClass();
		FishEditor::AssetImporter::Serialize(archive);
		archive << FishEngine::make_nvp("m_allowAlphaSplitting", m_allowAlphaSplitting); // bool
		archive << FishEngine::make_nvp("m_alphaIsTransparency", m_alphaIsTransparency); // bool
		archive << FishEngine::make_nvp("m_alphaSource", m_alphaSource); // FishEditor::TextureImporterAlphaSource
		archive << FishEngine::make_nvp("m_anisoLevel", m_anisoLevel); // int
		archive << FishEngine::make_nvp("m_borderMipmap", m_borderMipmap); // bool
		archive << FishEngine::make_nvp("m_compressionQuality", m_compressionQuality); // int
		archive << FishEngine::make_nvp("m_convertToNormalmap", m_convertToNormalmap); // bool
		archive << FishEngine::make_nvp("m_crunchedCompression", m_crunchedCompression); // bool
		archive << FishEngine::make_nvp("m_fadeout", m_fadeout); // bool
		archive << FishEngine::make_nvp("m_heightmapScale", m_heightmapScale); // float
		archive << FishEngine::make_nvp("m_generateCubemap", m_generateCubemap); // FishEditor::TextureImporterGenerateCubemap
		archive << FishEngine::make_nvp("m_textureType", m_textureType); // FishEditor::TextureImporterType
		archive << FishEngine::make_nvp("m_textureShape", m_textureShape); // FishEditor::TextureImporterShape
		archive << FishEngine::make_nvp("m_textureSettings", m_textureSettings); // FishEditor::TextureSettings
		archive << FishEngine::make_nvp("m_sRGBTexture", m_sRGBTexture); // bool
		archive << FishEngine::make_nvp("m_isReadable", m_isReadable); // bool
		archive << FishEngine::make_nvp("m_mipmapEnabled", m_mipmapEnabled); // bool
		archive << FishEngine::make_nvp("m_npotScale", m_npotScale); // FishEditor::TextureImporterNPOTScale
		//archive.EndClass();
	}

	void FishEditor::TextureImporter::Deserialize ( FishEngine::InputArchive & archive )
	{
		//archive.BeginClass(2);
		FishEditor::AssetImporter::Deserialize(archive);
		archive >> FishEngine::make_nvp("m_allowAlphaSplitting", m_allowAlphaSplitting); // bool
		archive >> FishEngine::make_nvp("m_alphaIsTransparency", m_alphaIsTransparency); // bool
		archive >> FishEngine::make_nvp("m_alphaSource", m_alphaSource); // FishEditor::TextureImporterAlphaSource
		archive >> FishEngine::make_nvp("m_anisoLevel", m_anisoLevel); // int
		archive >> FishEngine::make_nvp("m_borderMipmap", m_borderMipmap); // bool
		archive >> FishEngine::make_nvp("m_compressionQuality", m_compressionQuality); // int
		archive >> FishEngine::make_nvp("m_convertToNormalmap", m_convertToNormalmap); // bool
		archive >> FishEngine::make_nvp("m_crunchedCompression", m_crunchedCompression); // bool
		archive >> FishEngine::make_nvp("m_fadeout", m_fadeout); // bool
		archive >> FishEngine::make_nvp("m_heightmapScale", m_heightmapScale); // float
		archive >> FishEngine::make_nvp("m_generateCubemap", m_generateCubemap); // FishEditor::TextureImporterGenerateCubemap
		archive >> FishEngine::make_nvp("m_textureType", m_textureType); // FishEditor::TextureImporterType
		archive >> FishEngine::make_nvp("m_textureShape", m_textureShape); // FishEditor::TextureImporterShape
		archive >> FishEngine::make_nvp("m_textureSettings", m_textureSettings); // FishEditor::TextureSettings
		archive >> FishEngine::make_nvp("m_sRGBTexture", m_sRGBTexture); // bool
		archive >> FishEngine::make_nvp("m_isReadable", m_isReadable); // bool
		archive >> FishEngine::make_nvp("m_mipmapEnabled", m_mipmapEnabled); // bool
		archive >> FishEngine::make_nvp("m_npotScale", m_npotScale); // FishEditor::TextureImporterNPOTScale
		//archive.EndClass();
	}



} // namespace FishEditor