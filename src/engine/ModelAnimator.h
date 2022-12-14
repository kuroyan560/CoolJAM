#pragma once
#include<memory>
#include<string>
#include<list>
#include<array>
#include"KuroMath.h"
#include"Skeleton.h"
#include<vector>
#include"Transform.h"
class Model;
class ConstantBuffer;

class ModelAnimator
{
	static const int MAX_BONE_NUM = 256;

	//対応するスケルトンの参照
	std::weak_ptr<Skeleton>attachSkelton;
	//ボーンのローカル行列
	std::shared_ptr<ConstantBuffer>boneBuff;
	//ボーン行列（GPU送信用）
	std::array<Matrix, MAX_BONE_NUM>boneMatricies;
	//各ボーンのTransform
	std::vector<Transform>m_boneTransform;

	struct PlayAnimation
	{
		std::string name;	//アニメーション名
		float past = 0;	//経過フレーム(スローモーションなど考慮して浮動小数点)
		bool loop;
		bool finish = false;

		PlayAnimation(const std::string& Name, const bool& Loop) :name(Name), loop(Loop) {}
	};

	std::list<PlayAnimation>playAnimations;

	//ボーンTransform一括計算
	void CalculateTransform(Transform& BoneTransform, const Skeleton::BoneAnimation& BoneAnim, const float& Frame, bool& FinishFlg);
	void BoneMatrixRecursive(const int& BoneIdx, const float& Past, bool* Finish, Skeleton::ModelAnimation& Anim);

public:
	float speed = 1.0f;
	bool stop = false;

	ModelAnimator() {}
	ModelAnimator(std::weak_ptr<Model>Model);
	void Attach(std::weak_ptr<Model>Model);

	void Reset();

	//アニメーション再生
	void Play(const std::string& AnimationName, const bool& Loop, const bool& Blend);
	//指定のアニメーションが現在再生中か
	bool IsPlay(const std::string& AnimationName)
	{
		auto result = std::find_if(playAnimations.begin(), playAnimations.end(), [AnimationName](PlayAnimation& Anim)
			{
				return AnimationName.compare(Anim.name) == 0;
			});
		return result != playAnimations.end();
	}
	void Update();

	const std::shared_ptr<ConstantBuffer>& GetBoneMatBuff() { return boneBuff; }
	const Transform& GetBoneTransform(const std::string& BoneName);
};