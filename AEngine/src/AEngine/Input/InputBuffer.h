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
		InputBuffer() = default;

//--------------------------------------------------------------------------------
// Polling
//--------------------------------------------------------------------------------
			/**
			 * \brief Gets the state of a key
			 * \param[in] key The key to get the state of
			 * \returns The state of the key
			*/
		AEInputState GetKey(AEKey key) const;
			/**
			 * \brief Returns true if the key is down
			 * \param[in] key The key to check
			 * \returns True if the key is down, false otherwise
			 * \details
			 * This returns true if the key is either pressed or repeated
			*/
		bool IsKeyDown(AEKey key) const;
			/**
			 * \brief Returns true if the key is up
			 * \param[in] key The key to check
			 * \returns True if the key is up, false otherwise
			 * \details
			 * This returns true if the key is released
			*/
		bool IsKeyUp(AEKey key) const;

			/**
			 * \brief Gets the state of a mouse button
			 * \param[in] button The mouse button to get the state of
			 * \returns The state of the mouse button
			*/
		AEInputState GetMouseButton(AEMouse button) const;
			/**
			 * \brief Returns true if the mouse button is down
			 * \param[in] button The mouse button to check
			 * \returns True if the mouse button is down, false otherwise
			 * \details
			 * This returns true if the mouse button is either pressed or repeated
			*/
		bool IsMouseButtonDown(AEMouse button) const;
			/**
			 * \brief Returns true if the mouse button is up
			 * \param[in] button The mouse button to check
			 * \returns True if the mouse button is up, false otherwise
			 * \details
			 * This returns true if the mouse button is released
			*/
		bool IsMouseButtonUp(AEMouse button) const;


			/**
			 * \brief Gets the mouse position
			 * \returns The mouse position
			*/
		Math::vec2 GetMousePosition() const;
			/**
			 * \brief Gets the mouse delta
			 * \returns The mouse delta
			*/
		Math::vec2 GetMouseDelta() const;
			/**
			 * \brief Gets the mouse scroll
			 * \returns The mouse scroll
			*/
		Math::vec2 GetMouseScroll() const;

//--------------------------------------------------------------------------------
// Updaing
//--------------------------------------------------------------------------------
			/**
			 * \brief Resets the input buffer to its default state
			 * \details
			 * This sets all keys and mouse buttons to released
			 * and sets the mouse position, delta and scroll to 0
			*/
		void Reset();
			/**
			 * \brief Sets the state of a key
			 * \param[in] key The key to set the state of
			 * \param[in] state The state to set the key to
			*/
		void SetKeyState(AEKey key, AEInputState state);
			/**
			 * \brief Sets the state of a mouse button
			 * \param[in] button The mouse button to set the state of
			 * \param[in] state The state to set the mouse button to
			*/
		void SetMouseButtonState(AEMouse button, AEInputState state);
			/**
			 * \brief Sets the mouse position and delta
			 * \param[in] position The mouse position to set
			 * \param[in] delta The mouse delta to set
			*/
		void SetMouseState(const Math::vec2& position, const Math::vec2& delta);
			/**
			 * \brief Sets the mouse scroll
			 * \param[in] scroll The mouse scroll to set
			*/
		void SetMouseScroll(const Math::vec2& scroll);

		InputBuffer& operator=(const InputBuffer& rhs);

	private:
		// keys
		std::array<AEInputState, static_cast<Size_t>(AEKey::INVALID)> m_keyState{};
		std::array<AEInputState, static_cast<Size_t>(AEMouse::INVALID)> m_mouseState{};

		// mouse
		Math::vec2 m_mousePosition{ 0.0f, 0.0f };
		Math::vec2 m_mouseDelta{ 0.0f, 0.0f };
		Math::vec2 m_mouseScroll{ 0.0f, 0.0f };
	};
}
