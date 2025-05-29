#include "SceneFactory.h"
#include "TitleScene.h"  // TitleScene をインクルード
#include "TutorialScene.h"
#include "GamePlayScene.h" // GamePlayScene をインクルード
#include "GameClearScene.h"
#include "GameOverScene.h"

#include "TuboScene.h"
#include "AkimotoScene.h"
#include "SatouScene.h"


/// -------------------------------------------------------------
///				　		    シーン生成
/// -------------------------------------------------------------
std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    // 次のシーンを生成
    std::unique_ptr<BaseScene> newScene = nullptr;

    if (sceneName == "TitleScene")
    {
        return std::make_unique<TitleScene>();
    }
    if (sceneName == "TutorialScene") {
        return std::make_unique<TutorialScene>();
    }
    else if (sceneName == "GamePlayScene")
    {
        return std::make_unique<GamePlayScene>();
    }
    else if (sceneName == "GameClearScene")
    {
        return std::make_unique<GameClearScene>();
    }
    else if (sceneName == "GameOverScene")
    {
        return std::make_unique<GameOverScene>();
    }
	else if (sceneName == "TuboScene")
	{
		return std::make_unique<TuboScene>();
	}
    else if (sceneName == "AkimotoScene")
    {
		return std::make_unique<AkimotoScene>();
	}
    else if (sceneName == "SatouScene")
    {
        return std::make_unique<SatouScene>();
    }

    throw std::runtime_error("Unknown scene name: " + sceneName);
}
