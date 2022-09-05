#include "GaussianBlur.h"
#include"KuroEngine.h"
#include"DrawFunc2D.h"

std::shared_ptr<ComputePipeline>GaussianBlur::s_xBlurPipeline;	
std::shared_ptr<ComputePipeline>GaussianBlur::s_yBlurPipeline;	
std::shared_ptr<ComputePipeline>GaussianBlur::s_finalPipeline;

void GaussianBlur::GeneratePipeline()
{
    if (!s_xBlurPipeline)
    {
        std::vector<RootParam>rootParam =
        {
            RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�d�݃e�[�u��"),
            RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�e�N�X�`�����"),
            RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"�\�[�X�摜�o�b�t�@"),
            RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_UAV,"�`�����ݐ�o�b�t�@")
        };

        auto cs = D3D12App::Instance()->CompileShader("resource/engine/GaussianBlur.hlsl", "XBlur", "cs_6_4");
        s_xBlurPipeline = D3D12App::Instance()->GenerateComputePipeline(cs, rootParam, { WrappedSampler(false, true) });
        cs = D3D12App::Instance()->CompileShader("resource/engine/GaussianBlur.hlsl", "YBlur", "cs_6_4");
        s_yBlurPipeline = D3D12App::Instance()->GenerateComputePipeline(cs, rootParam, { WrappedSampler(false, true) });
        cs = D3D12App::Instance()->CompileShader("resource/engine/GaussianBlur.hlsl", "Final", "cs_6_4");
        s_finalPipeline = D3D12App::Instance()->GenerateComputePipeline(cs, rootParam, { WrappedSampler(false, true) });
    }
}

GaussianBlur::GaussianBlur(const Vec2<int>& Size, const DXGI_FORMAT& Format, const float& BlurPower)
{
    GeneratePipeline();

    //�d�݃e�[�u���萔�o�b�t�@
    m_weightConstBuff = D3D12App::Instance()->GenerateConstantBuffer(sizeof(float), s_weightNum, nullptr, "GaussianBlur - weight");
    SetBlurPower(BlurPower);

    //�e�N�X�`�����
    struct TexInfo
    {
        Vec2<int>sourceTexSize;
        Vec2<int>xBlurTexSize;
        Vec2<int>yBlurTexSize;
        Vec2<int>pad;
    }texInfo;
    texInfo.sourceTexSize = Size;
    texInfo.xBlurTexSize = { Size.x / 2,Size.y };
    texInfo.yBlurTexSize = { Size.x / 2,Size.y / 2 };
    m_texInfoConstBuff = D3D12App::Instance()->GenerateConstantBuffer(sizeof(TexInfo), 1, &texInfo, "GaussianBlur - TexInfo");

    //�c���u���[�̌��ʕ`���
    m_xBlurResult = D3D12App::Instance()->GenerateTextureBuffer(texInfo.xBlurTexSize, Format, "HorizontalBlur");
    m_yBlurResult = D3D12App::Instance()->GenerateTextureBuffer(texInfo.yBlurTexSize, Format, "VerticalBlur");

    //�ŏI�������ʕ`���
    m_finalResult = D3D12App::Instance()->GenerateTextureBuffer(texInfo.sourceTexSize, Format, "GaussianBlur");
}

void GaussianBlur::SetBlurPower(const float& BlurPower)
{
    // �d�݂̍��v���L�^����ϐ����`����
    float total = 0;

    // ��������K�E�X�֐���p���ďd�݂��v�Z���Ă���
    // ���[�v�ϐ���x����e�N�Z������̋���
    for (int x = 0; x < s_weightNum; x++)
    {
        m_weights[x] = expf(-0.5f * (float)(x * x) / BlurPower);
        total += 2.0f * m_weights[x];
    }

    // �d�݂̍��v�ŏ��Z���邱�ƂŁA�d�݂̍��v��1�ɂ��Ă���
    for (int i = 0; i < s_weightNum; i++)
    {
        m_weights[i] /= total;
    }

    m_weightConstBuff->Mapping(&m_weights[0]);
}

void GaussianBlur::Execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& CmdList, const std::shared_ptr<TextureBuffer>& SourceTex)
{
    const auto& sDesc = SourceTex->GetDesc();
    const auto& fDesc = m_finalResult->GetDesc();
    assert(sDesc.Width == fDesc.Width && sDesc.Height == fDesc.Height && sDesc.Format == fDesc.Format);

    static const int DIV = 4;

    //X�u���[
    s_xBlurPipeline->SetPipeline(CmdList);
    std::vector<RegisterDescriptorData>descDatas =
    {
        {m_weightConstBuff,CBV},
        {m_texInfoConstBuff,CBV},
        {SourceTex,SRV,D3D12_RESOURCE_STATE_GENERIC_READ},
        {m_xBlurResult,UAV},
    };
    for (int descIdx = 0; descIdx < descDatas.size(); ++descIdx)
    {
        descDatas[descIdx].SetAsCompute(CmdList, descIdx);
    }
    CmdList->Dispatch(
        static_cast<UINT>(ceil(m_xBlurResult->GetDesc().Width / DIV)),
        static_cast<UINT>(ceil(m_xBlurResult->GetDesc().Height / DIV)),
        1);

    //Y�u���[
    s_yBlurPipeline->SetPipeline(CmdList);
    descDatas[2].m_descData = m_xBlurResult;
    descDatas[3].m_descData = m_yBlurResult;
    for (int descIdx = 0; descIdx < descDatas.size(); ++descIdx)
    {
        descDatas[descIdx].SetAsCompute(CmdList, descIdx);
    }
    CmdList->Dispatch(
        static_cast<UINT>(ceil(m_yBlurResult->GetDesc().Width / DIV)),
        static_cast<UINT>(ceil(m_yBlurResult->GetDesc().Height / DIV)),
        1);

    //�ŏI���ʍ���
    s_finalPipeline->SetPipeline(CmdList);
    descDatas[2].m_descData = m_yBlurResult;
    descDatas[3].m_descData = m_finalResult;
    for (int descIdx = 0; descIdx < descDatas.size(); ++descIdx)
    {
        descDatas[descIdx].SetAsCompute(CmdList, descIdx);
    }
    CmdList->Dispatch(
        static_cast<UINT>(ceil(m_finalResult->GetDesc().Width / DIV)), 
        static_cast<UINT>(ceil(m_finalResult->GetDesc().Height / DIV)), 
        1);
}

void GaussianBlur::Register(const std::shared_ptr<TextureBuffer>& SourceTex)
{
    const auto sourceSize = SourceTex->GetGraphSize();
    const auto resultSize = m_finalResult->GetGraphSize();
    assert(sourceSize == resultSize);

    static const int DIV = 4;
    Vec3<int>threadNum;

    //X�u���[
    std::vector<RegisterDescriptorData>descDatas =
    {
        {m_weightConstBuff,CBV},
        {m_texInfoConstBuff,CBV},
        {SourceTex,SRV,D3D12_RESOURCE_STATE_GENERIC_READ},
        {m_xBlurResult,UAV},
    };

    KuroEngine::Instance()->Graphics().SetComputePipeline(s_xBlurPipeline);
    threadNum = { m_xBlurResult->GetGraphSize().x / DIV,m_xBlurResult->GetGraphSize().y / DIV, 1 };
    KuroEngine::Instance()->Graphics().Dispatch(threadNum, descDatas);

    //Y�u���[
    KuroEngine::Instance()->Graphics().SetComputePipeline(s_yBlurPipeline);
    threadNum = { m_yBlurResult->GetGraphSize().x / DIV, m_yBlurResult->GetGraphSize().y / DIV, 1 };
    descDatas[2].m_descData = m_xBlurResult;
    descDatas[3].m_descData = m_yBlurResult;
    KuroEngine::Instance()->Graphics().Dispatch(threadNum, descDatas);

    //�ŏI���ʍ���
    KuroEngine::Instance()->Graphics().SetComputePipeline(s_finalPipeline);
    threadNum = { m_finalResult->GetGraphSize().x / DIV, m_finalResult->GetGraphSize().y / DIV, 1 };
    descDatas[2].m_descData = m_yBlurResult;
    descDatas[3].m_descData = m_finalResult;
    KuroEngine::Instance()->Graphics().Dispatch(threadNum, descDatas);
}

#include"KuroEngine.h"
void GaussianBlur::DrawResult(const AlphaBlendMode& AlphaBlend)
{
    DrawFunc2D::DrawExtendGraph2D({ 0,0 }, WinApp::Instance()->GetExpandWinSize(), m_finalResult, AlphaBlend);
}
