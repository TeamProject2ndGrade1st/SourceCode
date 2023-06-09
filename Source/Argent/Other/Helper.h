// ReSharper disable All
#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <fbxsdk.h>
#include "d3dx12.h"
#include "../Graphic/Dx12/Command.h"
#include "Misc.h"
#include <cstdlib>
#include <string>
#include "Effekseer.h"



//シリアライズ
#include <cereal/archives/binary.hpp>

namespace Argent
{
	namespace Resource::Mesh
	{
		struct MeshResource;
	}

	namespace Component::Collision
	{
		struct HitResult;
	}

	namespace Helper
	{
		namespace Sprite
		{
			DirectX::XMFLOAT3 Rotate(const DirectX::XMFLOAT3& pos,const DirectX::XMFLOAT2& center, float angle);
			DirectX::XMFLOAT3 ToNDC(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT2& windowSize);
			DirectX::XMFLOAT2 CalcUVValue(const DirectX::XMFLOAT2& texPos, const DirectX::XMFLOAT2& texSize, const DirectX::XMFLOAT2& rowTextureSize);
			DirectX::XMFLOAT3 CalcScale(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT2& center, const DirectX::XMFLOAT2& scale);
		}

		namespace Math
		{
			inline size_t CalcAlignmentSize(size_t t)
			{
				return (t + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
			}

			std::vector<float> GetGaussianWeights(size_t count, float s);
		}

		namespace String
		{
			std::string GetTexturePathFromModelAndPath(const std::string& modelPath, const char* texturePath);
			
			//モデルパスからテクスチャパスへの変更

			// todo filesystemを使ったスマートな形へ変更する
			std::wstring GetWideStringFromString(const std::string& str);

			//ワイド文字列からマルチバイト文字列
			//ロケール依存
			inline std::string GetStringFromWideString(const std::wstring& src)
			{
				std::string ret{};
				size_t size{};
				const size_t destSize = src.length() * MB_CUR_MAX + 1;
				char* mbs = new char[destSize];
				wcstombs_s(&size, mbs, destSize,  src.c_str(), src.length());
			
				ret = mbs;
				delete[] mbs;

				return ret;
			}

			//拡張子を取得
			std::string GetExtension(const std::string& path);

			std::string ExtractFileName(const std::string& filePath, bool isNeedExtension);

			std::pair<std::string, std::string> SplitFileName(const std::string& path, const char splitter = '*');
		}

		namespace FBX
		{
			DirectX::XMFLOAT4X4 ToFloat4x4(const FbxAMatrix& fbxMatrix);
			DirectX::XMFLOAT3 Tofloat3(const FbxDouble3& fbxDouble);
			DirectX::XMFLOAT4 ToFloat4(const FbxDouble4& fbxDouble);
		}

		namespace Texture
		{
			HRESULT LoadTexture(ID3D12Device* device, Argent::Dx12::CommandBundle* cmdBundle, Argent::Dx12::CommandQueue* cmdQueue, const wchar_t* filepath, ID3D12Resource** resource);
		}

		namespace Dx12
		{
			namespace Blend
			{
				enum class BlendMode
				{
					bNone,
					bAlpha,
					bAdd,
					bSubtract,
					bMultiply,
					bLighten,
					bDarker,
					bNumBlendType,
				};

				D3D12_RENDER_TARGET_BLEND_DESC GenerateRenderTargetBlendDesc(BlendMode mode);
			}

			namespace Sampler
			{
				enum class FilterMode
				{
					fPoint,
					fLinear,
					fAnisotropic,
				};

				enum class WrapMode
				{
					wRepeat,
					wClamp,
					wMirror,
					wMirrorOnce,
					wBorder,
				};
				D3D12_STATIC_SAMPLER_DESC GenerateSamplerDesc(FilterMode fMode, WrapMode wMode,
					D3D12_STATIC_BORDER_COLOR borderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, FLOAT maxLOD = D3D12_FLOAT32_MAX, FLOAT minLOD = 0.0f);
			}

			namespace Rasterizer
			{
				//hack 引数を入れるように
				inline D3D12_RASTERIZER_DESC Generate()
				{
					D3D12_RASTERIZER_DESC desc{};
					desc.MultisampleEnable = FALSE;
					desc.FrontCounterClockwise = TRUE;
					desc.CullMode = D3D12_CULL_MODE_BACK;
					desc.FillMode = D3D12_FILL_MODE_SOLID;
					desc.DepthClipEnable = TRUE;
					return desc;
				}
			}


			namespace Buffer
			{
				template <typename T, class InIt, class OutIt>
				void CreateBuffer(ID3D12Device* device, UINT numBufferElements, InIt begin, OutIt end, T** map, ID3D12Resource** buffer, bool isUnMap = true, bool isNeedCopy = true)
				{
					const D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
					const D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(numBufferElements * sizeof(T));

					HRESULT hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
						nullptr, IID_PPV_ARGS(buffer));
					_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

					hr = (*buffer)->Map(0, nullptr, reinterpret_cast<void**>(map));
					_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

					if (isNeedCopy)
						std::copy(begin, end, (*map));

					if (isUnMap) (*buffer)->Unmap(0, nullptr);
				}

				/**
				 * \brief 頂点バッファを作成しビューを返す
				 * \tparam T 頂点構造体を指定
				 * \tparam InIt 指定必要なし
				 * \tparam OutIt 指定必要なし
				 * \param device
				 * \param numVertex 頂点配列の要素数
				 * \param begin 頂点配列の先頭　std::begin or iterator::begin
				 * \param end 頂点配列の末尾　std::end or iterator::end
				 * \param map 頂点構造体ポインタのアドレス
				 * \param buffer
				 * \param isUnMap unmapするかどうか　スプライトの場合はしない　それ以外はする
				 * \return 作成したビュー
				 */
				template <typename T, class InIt, class OutIt>
				D3D12_VERTEX_BUFFER_VIEW CreateVertex(ID3D12Device* device, UINT numVertex, InIt begin, OutIt end, T** map, ID3D12Resource** buffer, bool isUnMap = true)
				{
					CreateBuffer<T>(device, numVertex, begin, end, map, buffer, isUnMap);

					D3D12_VERTEX_BUFFER_VIEW view{};
					view.BufferLocation = (*buffer)->GetGPUVirtualAddress();
					view.SizeInBytes = static_cast<UINT>((*buffer)->GetDesc().Width);
					view.StrideInBytes = sizeof(T);
					return view;
				}

				/**
				 * \brief
				 * \tparam T インデックス構造体を指定　デフォルトはuint32_t
				 * \tparam InIt 指定必要なし
				 * \tparam OutIt 指定必要なし
				 * \param device
				 * \param numIndex インデックス配列の要素数
				 * \param begin 配列の先頭
				 * \param end 配列の末尾
				 * \param buffer
				 * \param format テンプレートのTを変更していないなら変更の必要なし
				 * \return
				 */
				template <class T = uint32_t, class InIt, class OutIt>
				D3D12_INDEX_BUFFER_VIEW CreateIndex(ID3D12Device* device, UINT numIndex, InIt begin, OutIt end, ID3D12Resource** buffer, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT)
				{
					T* map{};
					CreateBuffer<T>(device, numIndex, begin, end, &map, buffer, true);
					D3D12_INDEX_BUFFER_VIEW view{};
					view.BufferLocation = (*buffer)->GetGPUVirtualAddress();
					view.Format = format;
					view.SizeInBytes = static_cast<UINT>((*buffer)->GetDesc().Width);
					return view;
				}

				template <class T>
				void CreateConstant(ID3D12Device* device, T** map, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource** buffer)
				{
					HRESULT hr{ S_OK };
					const CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
					const CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(Helper::Math::CalcAlignmentSize(sizeof(T)));

					hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
						nullptr, IID_PPV_ARGS(buffer));
					_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));


					hr = (*buffer)->Map(0, nullptr, reinterpret_cast<void**>(map));
					DirectX::XMStoreFloat4x4(&(*map)->world, DirectX::XMMatrixIdentity());

					D3D12_CONSTANT_BUFFER_VIEW_DESC viewDesc{};
					viewDesc.BufferLocation = (*buffer)->GetGPUVirtualAddress();
					viewDesc.SizeInBytes = static_cast<UINT>((*buffer)->GetDesc().Width);
					device->CreateConstantBufferView(&viewDesc, cpuHandle);
				}
			}

			namespace InputElement
			{
				inline D3D12_INPUT_ELEMENT_DESC GenerateInputLayoutDesc(LPCSTR semanticName, DXGI_FORMAT format,
					UINT semanticIndex = 0, UINT inputSlot = 0, UINT alignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT,
					D3D12_INPUT_CLASSIFICATION inputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, UINT instanceDataStepRate = 0)
				{
					const D3D12_INPUT_ELEMENT_DESC desc{ semanticName, semanticIndex, format, inputSlot, alignedByteOffset,
						inputSlotClass, instanceDataStepRate };
					return desc;
				}
			}

			namespace DescriptorRange
			{
				inline D3D12_DESCRIPTOR_RANGE Generate(UINT baseShaderRegister, UINT numDescriptor, D3D12_DESCRIPTOR_RANGE_TYPE type,
					UINT offsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
				{
					D3D12_DESCRIPTOR_RANGE range{};
					range.BaseShaderRegister = baseShaderRegister;
					range.NumDescriptors = numDescriptor;
					range.RangeType = type;
					range.OffsetInDescriptorsFromTableStart = offsetInDescriptorsFromTableStart;
					return range;
				}
			}

			namespace RootParameter
			{
				inline D3D12_ROOT_PARAMETER Generate(UINT numDescriptorRanges, const D3D12_DESCRIPTOR_RANGE* pRange,
					D3D12_SHADER_VISIBILITY shaderVisibility)
				{
					D3D12_ROOT_PARAMETER param{};
					param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
					param.ShaderVisibility = shaderVisibility;
					param.DescriptorTable.NumDescriptorRanges = numDescriptorRanges;
					param.DescriptorTable.pDescriptorRanges = pRange;
					return param;
				}
			}
		}

		namespace Effect
		{
			inline Effekseer::Vector3D ToVector3D(const DirectX::XMFLOAT3& f)
			{
				Effekseer::Vector3D ret{};
				ret.X = f.x;
				ret.Y = f.y;
				ret.Z = f.z;
				return ret;
			}
		}

		namespace Cereal
		{
			template<class T>
			void serialize(T& archive, DirectX::XMFLOAT2& v)
			{
				archive(
				cereal::make_nvp("x", v.x),
				cereal::make_nvp("y", v.y));
			}

			template<class T>
			void serialize(T& archive, DirectX::XMFLOAT3& v)
			{
				archive(
				cereal::make_nvp("x", v.x),
				cereal::make_nvp("y", v.y),
				cereal::make_nvp("z", v.z));
			}

			template<class T>
			void serialize(T& archive, DirectX::XMFLOAT4& v)
			{
				archive(
				cereal::make_nvp("x", v.x),
				cereal::make_nvp("y", v.y),
				cereal::make_nvp("z", v.z),
				cereal::make_nvp("w", v.w));
			}
			
			template<class T>
			void serialize(T& archive, DirectX::XMFLOAT4X4& m)
			{
				archive(
				cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12),
				cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
				cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22),
				cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
				cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32),
				cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
				cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42),
				cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
				);
			}
		}

		namespace Collision
		{
			inline bool IntersectSphereVsSphere(
				const DirectX::XMFLOAT3& positionA, 
				float radiusA, 
				const DirectX::XMFLOAT3& positionB, 
				float radiusB, 
				DirectX::XMFLOAT3& outPositionB)
			{
				//A→Bの単位ベクトルを算出
				DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);	//Aベクトル
				DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);	//Bベクトル
				DirectX::XMVECTOR Vec		= DirectX::XMVectorSubtract(PositionB, PositionA);//ABベクトル
				DirectX::XMVECTOR VecNormal = DirectX::XMVector3Normalize(Vec);
				DirectX::XMVECTOR LengthSq	= DirectX::XMVector3Length(Vec);		//ABベクトルの大きさ
				float lengthSq;
				DirectX::XMStoreFloat(&lengthSq, LengthSq);

				//距離判定
				float range = radiusA + radiusB;
				if(lengthSq > range)
				{
					return false;
				}

				//AがBを押し出す
				Vec = DirectX::XMVectorScale(VecNormal, range);
				PositionB = DirectX::XMVectorAdd(Vec, PositionA);
				DirectX::XMStoreFloat3(&outPositionB, PositionB);

				return true;
			}

			

			bool IntersectRayVsModel(
				const DirectX::XMFLOAT3& start,
				const DirectX::XMFLOAT3& end,
				const Argent::Resource::Mesh::MeshResource& meshResource,
				const DirectX::XMMATRIX& worldTransform,
				Component::Collision::HitResult& result);


		}
	};
}

