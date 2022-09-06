#pragma once
#include<memory>
#include<wrl.h>
#include"D3D12Data.h"
#include<array>

class GaussianBlur
{
private:
	static const int THREAD_DIV = 8;
	static enum PROCESS { X_BLUR, Y_BLUR, FINAL, PROCESS_NUM };
	static const int s_weightNum = 8;
	static std::array<std::shared_ptr<ComputePipeline>, PROCESS_NUM>s_csPipeline;
	static void GeneratePipeline();

private:
	//重みテーブル
	float m_weights[s_weightNum];

	//重みテーブルの定数バッファ
	std::shared_ptr<ConstantBuffer>m_weightConstBuff;
	//テクスチャ情報の定数バッファ
	std::shared_ptr<ConstantBuffer>m_texInfoConstBuff;
	//ブラーの結果
	std::array<std::shared_ptr<TextureBuffer>, PROCESS_NUM>m_blurResult;

public:
	//ソース画像 & 結果描画先設定、ぼかし強さ
	GaussianBlur(const Vec2<int>& Size, const DXGI_FORMAT& Format, const float& BlurPower = 8.0f);
	//ボケ具合
	void SetBlurPower(const float& BlurPower);
	//即時実行
	void Execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& CmdList, const std::shared_ptr<TextureBuffer>& SourceTex);
	//グラフィックスマネージャに登録
	void Register(const std::shared_ptr<TextureBuffer>& SourceTex);

	//結果のテクスチャ取得
	std::shared_ptr<TextureBuffer>& GetResultTex() { return m_blurResult[FINAL]; }

	//ウィンドウサイズで結果を描画
	void DrawResult(const AlphaBlendMode& AlphaBlend);
};