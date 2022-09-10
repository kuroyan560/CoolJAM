#include "GameTimer.h"
#include"../Engine/ImguiApp.h"
#include"../Common/KuroMath.h"
#include"WinApp.h"
#include"AudioApp.h"
#include"../user/KazDrawFunc.h"
#include"../engine/Common/KuroMath.h"


GameTimer::GameTimer()
{
	startFlag = false;
	timer = -1;
	flame = -1;
	timerSize = 0;
	timerAlpha = 255;
	isLessThan5SecondsLeft = false;
	centerCountDownSize = 0;
	centerCoundDownAlpha = 0;

	number.resize(12);
	texSize = { 64,44 };

	D3D12App::Instance()->GenerateTextureBuffer(textureBufferArray.data(), "resource/user/num.png", 12, { 12, 1 });
	//TexHandleMgr::LoadDivGraph("resource/ChainCombat/UI/num.png", 12, { 12, 1 }, number.data());

	//スコア無効、タイマーを中心に描画
	timerPos.x = 0.0f - 150.0f;
	timerPos.y = 46.0f;

	//countDownSE[0] = AudioApp::Instance()->LoadAudio("resource/ChainCombat/sound/voice/Voice_1.wav", 0.2f);
	//countDownSE[1] = AudioApp::Instance()->LoadAudio("resource/ChainCombat/sound/voice/Voice_2.wav", 0.2f);
	//countDownSE[2] = AudioApp::Instance()->LoadAudio("resource/ChainCombat/sound/voice/Voice_3.wav", 0.2f);
	//countDownSE[3] = AudioApp::Instance()->LoadAudio("resource/ChainCombat/sound/voice/Voice_4.wav", 0.2f);
	//countDownSE[4] = AudioApp::Instance()->LoadAudio("resource/ChainCombat/sound/voice/Voice_5.wav", 0.2f);

}

void GameTimer::Init(int TIME)
{
	startFlag = false;
	timeUpFlag = false;
	isLessThan5SecondsLeft = false;
	timer = TIME;
	flame = 0;
	timerSize = 0;
	timerAlpha = 255;
	centerCountDownSize = 0;
	centerCoundDownAlpha = 0;
	timeUpInterval = 0;

	countDownNum = 3;

	countDownFlag = false;
	int minite = 0;
	int second = 0;
	int t = 0;

	if (60 <= timer)
	{
		minite = timer;
		second = 0;
		t = 0;
		for (; 60 <= minite;)
		{
			minite -= 60;
			if (minite <= 60)
			{
				second = minite;
			}
			++t;
		}
	}
	else
	{
		minite = 0;
		second = timer;
		t = 0;
	}


	//分
	minitueHandle = CountNumber(t);
	timeHandle = CountNumber(second);
	miriHandle = CountNumber(flame);

	finishTimer = 0;

	const Vec2<float>size = { 600,200 };
	Vec2<float> p;
	p.x = timerPos.x + (size.x / 2);
	p.y = timerPos.y + (size.y / 2) + 50;

	//timerPos = p;

	counDownFinishFlag = false;

	//登場演出のため最初は動かない
	interruput = true;

	startEasePosX = 0.0f;
	endEasePosX = 0.0f;
	startRate = 0.0f;
	endRate = 0.0f;

	finishAllEffectFlag = false;
}

void GameTimer::Finalize()
{
}

void GameTimer::Update()
{
	if (interruput && false) {

		// タイマーが計測していない間はサイズを0に近づける。

		// UIのサイズを0に近づける。
		timerSize += (-timerSize) / 5.0f;

		// UIのアルファ値を0に近づける。
		timerAlpha += (-timerAlpha) / 5.0f;

		centerCountDownSize -= centerCountDownSize / 5.0f;
		centerCoundDownAlpha -= centerCoundDownAlpha / 5.0f;

		return;

	}


	if (startFlag)
	{
		if (startRate <= 1.0f)
		{
			startRate += 1.0f / 30.0f;
		}
		if (1.0f <= startRate)
		{
			startRate = 1.0f;
		}
	}
	if (timeUpFlag)
	{
		if (endRate <= 1.0f)
		{
			endRate += 1.0f / 30.0f;
		}
		if (1.0f <= endRate)
		{
			endRate = 1.0f;
		}
	}

	//時間切れ
	timeUpFlag = startFlag && timer <= 0 && flame <= 0;
	if (timeUpFlag)
	{
		++timeUpInterval;
	}
	if (120 <= timeUpInterval)
	{
		finishAllEffectFlag = true;
	}


	startFlag = countDownFlag;
	if (1.0f <= startRate && !timeUpFlag)
	{
		//時間のカウント
		if (0 < flame)
		{
			flame--;
		}
		else
		{
			timer--;
			flame = 59;

			// タイマーが5以下だったらサウンドを鳴らす。
			if (timer <= 5 && timer != 0) {

				//AudioApp::Instance()->PlayWave(countDownSE[timer - 1]);
				isLessThan5SecondsLeft = true;

				centerCoundDownAlpha = 0;
				centerCountDownSize = 10.0f;
			}

			timerSize = 1.8f;
		}

		int minite = timer;
		int t = 0;
		for (; 60 <= minite;)
		{
			minite -= 60;
			++t;
		}
		//分
		minitueHandle = CountNumber(t);

		//秒
		timeHandle = CountNumber(timer - 60 * t);

		//ミリ秒
		int tmp = (flame / 60) * 100;
		miriHandle = CountNumber(tmp);

	}

	{

		// UIのサイズをデフォルトに近づける。
		timerSize += (OFFSET_SIZE - timerSize) / 5.0f;

		// UIのアルファ値を255に近づける。
		timerAlpha += (255 - timerAlpha) / 5.0f;

	}



}

void GameTimer::Draw()
{
	{
		float easePosX = 0.0;
		float winSizeX = WinApp::Instance()->GetExpandWinSize().x / 2.0f;
		if (startFlag)
		{
			startEasePosX = KuroMath::Ease(Out, Cubic, startRate, 0.0f, 1.0f) * winSizeX;
			easePosX = startEasePosX;
		}
		if (timeUpFlag)
		{
			endEasePosX = startEasePosX + KuroMath::Ease(In, Cubic, endRate, 0.0f, 1.0f) * (winSizeX + 200.0f);
			easePosX = endEasePosX;
		}



		Vec2<float>centralPos;
		int offset = 0;
		int offsetY = 15;
		//分
		for (int i = 0; i < minitueHandle.size(); i++)
		{
			offset = i;
			centralPos = { easePosX + timerPos.x + i * texSize.x, timerPos.y + offsetY };
			KazDrawFunc::DrawRotaGraph2D(centralPos, Vec2<float>(timerSize, timerSize), 0.0f, textureBufferArray[minitueHandle[i]], timerAlpha);
		}

		++offset;
		centralPos = { easePosX + timerPos.x + offset * texSize.x,timerPos.y + offsetY };
		KazDrawFunc::DrawRotaGraph2D(centralPos, Vec2<float>(timerSize, timerSize), 0.0f, textureBufferArray[10], timerAlpha);
		++offset;

		//秒
		for (int i = 0; i < timeHandle.size(); i++)
		{
			centralPos = { easePosX + timerPos.x + (offset + i) * texSize.x, timerPos.y + offsetY };
			KazDrawFunc::DrawRotaGraph2D(centralPos, Vec2<float>(timerSize, timerSize), 0.0f, textureBufferArray[timeHandle[i]], timerAlpha);
		}

	}
}

void GameTimer::Start()
{
	countDownFlag = true;
}

int GameTimer::GetFlame()
{
	return timer;
}

int GameTimer::GetSeceond()
{
	float second = timer / 60;
	return second;
}

bool GameTimer::TimeUpFlag()
{
	return timeUpFlag;
}

bool GameTimer::StartGame()
{
	return counDownFinishFlag;
}

bool GameTimer::FinishAllEffect()
{
	return finishAllEffectFlag;
}

void GameTimer::Debug()
{
	ImGui::Begin("GameTimer");
	ImGui::Text("Time:%d%d", timeHandle[0], timeHandle[1]);
	ImGui::End();
}

std::vector<int> GameTimer::CountNumber(int TIME)
{
	float score = TIME;
	std::vector<int> Number(2);
	Number[0] = -1;
	Number[1] = -1;

	int tmp = score;
	//スコア計算
	for (int i = 0; tmp > 0; i++)
	{
		float result = tmp % 10;
		//Number.push_back(result);
		Number[i] = result;
		tmp /= 10.0f;
	}
	//0埋め
	for (int i = 0; i < Number.size(); i++)
	{
		if (Number[i] == -1)
		{
			Number[i] = 0;
		}
	}
	std::reverse(Number.begin(), Number.end());
	return Number;
}

bool GameTimer::IsStart()
{
	return startFlag;
}