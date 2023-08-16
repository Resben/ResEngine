#pragma once
#include <array>
#include "AEngine/Core/Types.h"
#include "AEngine/Math/Math.h"
#include "KeyCodes.h"

namespace AEngine
{
	class InputBuffer
	{
	public:
			/**
			 * \brief Gets the singleton instance of the input system
			 * \return The singleton instance of the input system
			*/
		static InputBuffer& Instance();

//--------------------------------------------------------------------------------
// Exposed via Public API
//--------------------------------------------------------------------------------
			/**
			 * \copydoc Input::IsKeyPressed
			*/
		bool IsKeyPressed(AEKey key);
			/**
			 * \copydoc Input::IsKeyPressedNoRepeat
			*/
		bool IsKeyPressedNoRepeat(AEKey key);
			/**
			 * \copydoc Input::IsMouseButtonPressed
			*/
		bool IsMouseButtonPressed(AEMouse button);
			/**
			 * \copydoc Input::IsMouseButtonPressedNoRepeat
			*/
		bool IsMouseButtonPressedNoRepeat(AEMouse button);
			/**
			 * \copydoc Input::GetMousePosition
			*/
		Math::vec2 GetMousePosition();
			/**
			 * \copydoc Input::GetMouseDelta
			*/
		Math::vec2 GetMouseDelta();
			/**
			 * \copydoc Input::GetMouseScroll
			*/
		Math::vec2 GetMouseScroll();

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
		InputBuffer() = default;
		
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
