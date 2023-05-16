#pragma once
#include "AEngine/Math/Math.h"
#include "KeyCodes.h"
#include "Types.h"
#include <array>

namespace AEngine
{
	class InputImpl
	{
	public:
			/**
			 * \brief Gets the singleton instance of the input system
			 * \return The singleton instance of the input system
			*/
		static InputImpl& Instance();

//--------------------------------------------------------------------------------
// Exposed via Public API
//--------------------------------------------------------------------------------
			/**
			 * \copydoc Input::IsKeyPressed
			*/
		bool IsKeyPressedImpl(AEKey key);
			/**
			 * \copydoc Input::IsKeyPressedNoRepeat
			*/
		bool IsKeyPressedNoRepeatImpl(AEKey key);
			/**
			 * \copydoc Input::IsMouseButtonPressed
			*/
		bool IsMouseButtonPressedImpl(AEMouse button);
			/**
			 * \copydoc Input::IsMouseButtonPressedNoRepeat
			*/
		bool IsMouseButtonPressedNoRepeatImpl(AEMouse button);
			/**
			 * \copydoc Input::GetMousePosition
			*/
		Math::vec2 GetMousePositionImpl();
			/**
			 * \copydoc Input::GetMouseDelta
			*/
		Math::vec2 GetMouseDeltaImpl();
			/**
			 * \copydoc Input::GetMouseScroll
			*/
		Math::vec2 GetMouseScrollImpl();

//--------------------------------------------------------------------------------
// Internal use only
//--------------------------------------------------------------------------------
			/**
			 * \brief Updates the input state
			 * \note Called by the engine
			 *
			 * The engine calls this at the end of each frame before processing new events.
			 * This allows the input system to update its state before the next frame.
			*/
		void OnUpdate();
			/**
			 * \brief Sets the state of a key
			 * \param[in] key The key to set the state of
			 * \param[in] state The state to set the key to
			*/
		void SetKeyState(AEKey key, bool state);
			/**
			 * \brief Sets the state of a mouse button
			 * \param[in] button The mouse button to set the state of
			 * \param[in] state The state to set the mouse button to
			*/
		void SetMouseButtonState(AEMouse button, bool state);
			/**
			 * \brief Sets the mouse position
			 * \param[in] position The mouse position to set
			*/
		void SetMousePosition(const Math::vec2& position);
			/**
			 * \brief Sets the mouse scroll
			 * \param[in] scroll The mouse scroll to set
			*/
		void SetMouseScroll(const Math::vec2& scroll);

	private:
		InputImpl() = default;

		// keys
		std::array<bool, static_cast<Size_t>(AEKey::INVALID)> m_keyState{};
		std::array<bool, static_cast<Size_t>(AEKey::INVALID)> m_keyStateLast{};
		std::array<bool, static_cast<Size_t>(AEMouse::INVALID)> m_mouseButtonState{};
		std::array<bool, static_cast<Size_t>(AEMouse::INVALID)> m_mouseButtonStateLast{};

		// mouse
		Math::vec2 m_mousePosition{ 0.0f, 0.0f };
		Math::vec2 m_mousePositionLast{ 0.0f, 0.0f};
		Math::vec2 m_mouseScroll{ 0.0f, 0.0f };
	};
}
