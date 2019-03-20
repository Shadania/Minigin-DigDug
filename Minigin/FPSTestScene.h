#pragma once
#include "Scene.h"

namespace dae
{
	class TextComponent;

	class FPSTestScene final : public Scene
	{
	public:
		FPSTestScene();
		virtual ~FPSTestScene();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() const override;

		virtual void Init() override;
	private:
		std::shared_ptr<TextComponent> m_spFPSText;


	};
}