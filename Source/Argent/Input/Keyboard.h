#pragma once
#include <Windows.h>
#include <unordered_map>
#include <DirectXMath.h>


namespace Argent::Input
{
	enum class State
	{
		NONE,
		ENTER,
		PRESS,
		RELEASE,
	};

	class Keyboard
	{
	public:
		enum KeyCode : unsigned char
		{
			None = 0,

			Back = 0x8,
			Tab = 0x9,

			Enter = 0xd,

			Pause = 0x13,
			CapsLock = 0x14,
			Kana = 0x15,
			ImeOn = 0x16,

			Kanji = 0x19,

			ImeOff = 0x1a,
			Escape = 0x1b,
			ImeConvert = 0x1c,
			ImeNoConvert = 0x1d,

			Space = 0x20,
			PageUp = 0x21,
			PageDown = 0x22,
			End = 0x23,
			Home = 0x24,
			Left = 0x25,
			Up = 0x26,
			Right = 0x27,
			Down = 0x28,
			Select = 0x29,
			Print = 0x2a,
			Execute = 0x2b,
			PrintScreen = 0x2c,
			Insert = 0x2d,
			Delete = 0x2e,
			Help = 0x2f,
			D0 = 0x30,
			D1 = 0x31,
			D2 = 0x32,
			D3 = 0x33,
			D4 = 0x34,
			D5 = 0x35,
			D6 = 0x36,
			D7 = 0x37,
			D8 = 0x38,
			D9 = 0x39,

			A = 0x41,
			B = 0x42,
			C = 0x43,
			D = 0x44,
			E = 0x45,
			F = 0x46,
			G = 0x47,
			H = 0x48,
			I = 0x49,
			J = 0x4a,
			K = 0x4b,
			L = 0x4c,
			M = 0x4d,
			N = 0x4e,
			O = 0x4f,
			P = 0x50,
			Q = 0x51,
			R = 0x52,
			S = 0x53,
			T = 0x54,
			U = 0x55,
			V = 0x56,
			W = 0x57,
			X = 0x58,
			Y = 0x59,
			Z = 0x5a,
			LeftWindows = 0x5b,
			RightWindows = 0x5c,
			Apps = 0x5d,

			Sleep = 0x5f,
			NumPad0 = 0x60,
			NumPad1 = 0x61,
			NumPad2 = 0x62,
			NumPad3 = 0x63,
			NumPad4 = 0x64,
			NumPad5 = 0x65,
			NumPad6 = 0x66,
			NumPad7 = 0x67,
			NumPad8 = 0x68,
			NumPad9 = 0x69,
			Multiply = 0x6a,
			Add = 0x6b,
			Separator = 0x6c,
			Subtract = 0x6d,

			Decimal = 0x6e,
			Divide = 0x6f,
			F1 = 0x70,
			F2 = 0x71,
			F3 = 0x72,
			F4 = 0x73,
			F5 = 0x74,
			F6 = 0x75,
			F7 = 0x76,
			F8 = 0x77,
			F9 = 0x78,
			F10 = 0x79,
			F11 = 0x7a,
			F12 = 0x7b,
			F13 = 0x7c,
			F14 = 0x7d,
			F15 = 0x7e,
			F16 = 0x7f,
			F17 = 0x80,
			F18 = 0x81,
			F19 = 0x82,
			F20 = 0x83,
			F21 = 0x84,
			F22 = 0x85,
			F23 = 0x86,
			F24 = 0x87,

			NumLock = 0x90,
			Scroll = 0x91,

			LeftShift = 0xa0,
			RightShift = 0xa1,
			LeftControl = 0xa2,
			RightControl = 0xa3,
			LeftAlt = 0xa4,
			RightAlt = 0xa5,
			BrowserBack = 0xa6,
			BrowserForward = 0xa7,
			BrowserRefresh = 0xa8,
			BrowserStop = 0xa9,
			BrowserSearch = 0xaa,
			BrowserFavorites = 0xab,
			BrowserHome = 0xac,
			VolumeMute = 0xad,
			VolumeDown = 0xae,
			VolumeUp = 0xaf,
			MediaNextTrack = 0xb0,
			MediaPreviousTrack = 0xb1,
			MediaStop = 0xb2,
			MediaPlayPause = 0xb3,
			LaunchMail = 0xb4,
			SelectMedia = 0xb5,
			LaunchApplication1 = 0xb6,
			LaunchApplication2 = 0xb7,

			OemSemicolon = 0xba,
			OemPlus = 0xbb,
			OemComma = 0xbc,
			OemMinus = 0xbd,
			OemPeriod = 0xbe,
			OemQuestion = 0xbf,
			OemTilde = 0xc0,

			OemOpenBrackets = 0xdb,
			OemPipe = 0xdc,
			OemCloseBrackets = 0xdd,
			OemQuotes = 0xde,
			Oem8 = 0xdf,

			OemBackslash = 0xe2,

			ProcessKey = 0xe5,

			OemCopy = 0xf2,
			OemAuto = 0xf3,
			OemEnlW = 0xf4,

			Attn = 0xf6,
			Crsel = 0xf7,
			Exsel = 0xf8,
			EraseEof = 0xf9,
			Play = 0xfa,
			Zoom = 0xfb,

			Pa1 = 0xfd,
			OemClear = 0xfe,
		};

		Keyboard(const Keyboard&) = delete;
		Keyboard operator=(Keyboard&) = delete;

		static Keyboard& Instance()
		{
			static Keyboard instance;
			return instance;
		}
	private:
		
		Keyboard()
		{
			keyState[KeyCode::None					] = State::NONE;
			keyState[KeyCode::Back					] = State::NONE;
			keyState[KeyCode::Tab					] = State::NONE;
			keyState[KeyCode::Enter				] = State::NONE;
			keyState[KeyCode::Pause				] = State::NONE;
			keyState[KeyCode::CapsLock				] = State::NONE;
			keyState[KeyCode::Kana					] = State::NONE;
			keyState[KeyCode::ImeOn				] = State::NONE;
			keyState[KeyCode::Kanji				] = State::NONE;
			keyState[KeyCode::ImeOff				] = State::NONE;
			keyState[KeyCode::Escape				] = State::NONE;
			keyState[KeyCode::ImeConvert			] = State::NONE;
			keyState[KeyCode::ImeNoConvert			] = State::NONE;
			keyState[KeyCode::Space				] = State::NONE;
			keyState[KeyCode::PageUp				] = State::NONE;
			keyState[KeyCode::PageDown				] = State::NONE;
			keyState[KeyCode::End					] = State::NONE;
			keyState[KeyCode::Home					] = State::NONE;
			keyState[KeyCode::Left					] = State::NONE;
			keyState[KeyCode::Up					] = State::NONE;
			keyState[KeyCode::Right				] = State::NONE;
			keyState[KeyCode::Down					] = State::NONE;
			keyState[KeyCode::Select				] = State::NONE;
			keyState[KeyCode::Print				] = State::NONE;
			keyState[KeyCode::Execute				] = State::NONE;
			keyState[KeyCode::PrintScreen			] = State::NONE;
			keyState[KeyCode::Insert				] = State::NONE;
			keyState[KeyCode::Delete				] = State::NONE;
			keyState[KeyCode::Help					] = State::NONE;
			keyState[KeyCode::D0					] = State::NONE;
			keyState[KeyCode::D1					] = State::NONE;
			keyState[KeyCode::D2					] = State::NONE;
			keyState[KeyCode::D3					] = State::NONE;
			keyState[KeyCode::D4					] = State::NONE;
			keyState[KeyCode::D5					] = State::NONE;
			keyState[KeyCode::D6					] = State::NONE;
			keyState[KeyCode::D7					] = State::NONE;
			keyState[KeyCode::D8					] = State::NONE;
			keyState[KeyCode::D9					] = State::NONE;
			keyState[KeyCode::A					] = State::NONE;
			keyState[KeyCode::B					] = State::NONE;
			keyState[KeyCode::C					] = State::NONE;
			keyState[KeyCode::D					] = State::NONE;
			keyState[KeyCode::E					] = State::NONE;
			keyState[KeyCode::F					] = State::NONE;
			keyState[KeyCode::G					] = State::NONE;
			keyState[KeyCode::H					] = State::NONE;
			keyState[KeyCode::I					] = State::NONE;
			keyState[KeyCode::J					] = State::NONE;
			keyState[KeyCode::K					] = State::NONE;
			keyState[KeyCode::L					] = State::NONE;
			keyState[KeyCode::M					] = State::NONE;
			keyState[KeyCode::N					] = State::NONE;
			keyState[KeyCode::O					] = State::NONE;
			keyState[KeyCode::P					] = State::NONE;
			keyState[KeyCode::Q					] = State::NONE;
			keyState[KeyCode::R					] = State::NONE;
			keyState[KeyCode::S					] = State::NONE;
			keyState[KeyCode::T					] = State::NONE;
			keyState[KeyCode::U					] = State::NONE;
			keyState[KeyCode::V					] = State::NONE;
			keyState[KeyCode::W					] = State::NONE;
			keyState[KeyCode::X					] = State::NONE;
			keyState[KeyCode::Y					] = State::NONE;
			keyState[KeyCode::Z					] = State::NONE;
			keyState[KeyCode::LeftWindows			] = State::NONE;
			keyState[KeyCode::RightWindows			] = State::NONE;
			keyState[KeyCode::Apps					] = State::NONE;
			keyState[KeyCode::Sleep				] = State::NONE;
			keyState[KeyCode::NumPad0				] = State::NONE;
			keyState[KeyCode::NumPad1				] = State::NONE;
			keyState[KeyCode::NumPad2				] = State::NONE;
			keyState[KeyCode::NumPad3				] = State::NONE;
			keyState[KeyCode::NumPad4				] = State::NONE;
			keyState[KeyCode::NumPad5				] = State::NONE;
			keyState[KeyCode::NumPad6				] = State::NONE;
			keyState[KeyCode::NumPad7				] = State::NONE;
			keyState[KeyCode::NumPad8				] = State::NONE;
			keyState[KeyCode::NumPad9				] = State::NONE;
			keyState[KeyCode::Multiply				] = State::NONE;
			keyState[KeyCode::Add					] = State::NONE;
			keyState[KeyCode::Separator			] = State::NONE;
			keyState[KeyCode::Subtract				] = State::NONE;
			keyState[KeyCode::Decimal				] = State::NONE;
			keyState[KeyCode::Divide				] = State::NONE;
			keyState[KeyCode::F1					] = State::NONE;
			keyState[KeyCode::F2					] = State::NONE;
			keyState[KeyCode::F3					] = State::NONE;
			keyState[KeyCode::F4					] = State::NONE;
			keyState[KeyCode::F5					] = State::NONE;
			keyState[KeyCode::F6					] = State::NONE;
			keyState[KeyCode::F7					] = State::NONE;
			keyState[KeyCode::F8					] = State::NONE;
			keyState[KeyCode::F9					] = State::NONE;
			keyState[KeyCode::F10					] = State::NONE;
			keyState[KeyCode::F11					] = State::NONE;
			keyState[KeyCode::F12					] = State::NONE;
			keyState[KeyCode::F13					] = State::NONE;
			keyState[KeyCode::F14					] = State::NONE;
			keyState[KeyCode::F15					] = State::NONE;
			keyState[KeyCode::F16					] = State::NONE;
			keyState[KeyCode::F17					] = State::NONE;
			keyState[KeyCode::F18					] = State::NONE;
			keyState[KeyCode::F19					] = State::NONE;
			keyState[KeyCode::F20					] = State::NONE;
			keyState[KeyCode::F21					] = State::NONE;
			keyState[KeyCode::F22					] = State::NONE;
			keyState[KeyCode::F23					] = State::NONE;
			keyState[KeyCode::F24					] = State::NONE;
			keyState[KeyCode::NumLock				] = State::NONE;
			keyState[KeyCode::Scroll				] = State::NONE;
			keyState[KeyCode::LeftShift			] = State::NONE;
			keyState[KeyCode::RightShift			] = State::NONE;
			keyState[KeyCode::LeftControl			] = State::NONE;
			keyState[KeyCode::RightControl			] = State::NONE;
			keyState[KeyCode::LeftAlt				] = State::NONE;
			keyState[KeyCode::RightAlt				] = State::NONE;
			keyState[KeyCode::BrowserBack			] = State::NONE;
			keyState[KeyCode::BrowserForward		] = State::NONE;
			keyState[KeyCode::BrowserRefresh		] = State::NONE;
			keyState[KeyCode::BrowserStop			] = State::NONE;
			keyState[KeyCode::BrowserSearch		] = State::NONE;
			keyState[KeyCode::BrowserFavorites		] = State::NONE;
			keyState[KeyCode::BrowserHome			] = State::NONE;
			keyState[KeyCode::VolumeMute			] = State::NONE;
			keyState[KeyCode::VolumeDown			] = State::NONE;
			keyState[KeyCode::VolumeUp				] = State::NONE;
			keyState[KeyCode::MediaNextTrack		] = State::NONE;
			keyState[KeyCode::MediaPreviousTrack	] = State::NONE;
			keyState[KeyCode::MediaStop			] = State::NONE;
			keyState[KeyCode::MediaPlayPause		] = State::NONE;
			keyState[KeyCode::LaunchMail			] = State::NONE;
			keyState[KeyCode::SelectMedia			] = State::NONE;
			keyState[KeyCode::LaunchApplication1	] = State::NONE;
			keyState[KeyCode::LaunchApplication2	] = State::NONE;
			keyState[KeyCode::OemSemicolon			] = State::NONE;
			keyState[KeyCode::OemPlus				] = State::NONE;
			keyState[KeyCode::OemComma				] = State::NONE;
			keyState[KeyCode::OemMinus				] = State::NONE;
			keyState[KeyCode::OemPeriod			] = State::NONE;
			keyState[KeyCode::OemQuestion			] = State::NONE;
			keyState[KeyCode::OemTilde				] = State::NONE;
			keyState[KeyCode::OemOpenBrackets		] = State::NONE;
			keyState[KeyCode::OemPipe				] = State::NONE;
			keyState[KeyCode::OemCloseBrackets		] = State::NONE;
			keyState[KeyCode::OemQuotes			] = State::NONE;
			keyState[KeyCode::Oem8					] = State::NONE;
			keyState[KeyCode::OemBackslash			] = State::NONE;
			keyState[KeyCode::ProcessKey			] = State::NONE;
			keyState[KeyCode::OemCopy				] = State::NONE;
			keyState[KeyCode::OemAuto				] = State::NONE;
			keyState[KeyCode::OemEnlW				] = State::NONE;
			keyState[KeyCode::Attn					] = State::NONE;
			keyState[KeyCode::Crsel				] = State::NONE;
			keyState[KeyCode::Exsel				] = State::NONE;
			keyState[KeyCode::EraseEof				] = State::NONE;
			keyState[KeyCode::Play					] = State::NONE;
			keyState[KeyCode::Zoom					] = State::NONE;
			keyState[KeyCode::Pa1					] = State::NONE;
			keyState[KeyCode::OemClear				] = State::NONE;
		}
			
		~Keyboard() = default;


	public:

		void Update()
		{
			UpdateKeyStates();
		}

		void Clear()
		{
			for(auto it = keyState.begin(); it != keyState.end(); ++it)
			{
				(*it).second = State::NONE;
			}
		}

		/**
		 * \brief ‰Ÿ‚µ‚Ä‚¢‚éŠÔtrue‚ð•Ô‚·
		 * \param key 
		 * \return 
		 */
		bool GetKey(KeyCode key) const
		{
			return keyState.find(key)->second == State::PRESS;
		}

		/**
		 * \brief —£‚µ‚½uŠÔtrue‚ð•Ô‚·
		 * \param key 
		 * \return 
		 */
		bool GetKeyUp(KeyCode key) const
		{
			return keyState.find(key)->second == State::RELEASE;
		}

		/**
		 * \brief ‰Ÿ‚µ‚½uŠÔtrue‚ð•Ô‚·
		 * \param key 
		 * \return 
		 */
		bool GetKeyDown(KeyCode key) const
		{
			return keyState.find(key)->second == State::ENTER;
		}

	/*	State GetKeyState(KeyCode key) const
		{
			return keyState.find(key)->second;
		}*/

		void UpdateKeyStates()
		{
			for(auto it = keyState.begin(); it != keyState.end(); ++it)
			{
				const auto State = GetKeyState((*it).first);
				if(State & 0x8000)
				{
					switch ((*it).second)
					{
					case State::NONE:
						(*it).second = State::ENTER;
						break;

					case State::ENTER:
					case State::PRESS:
						(*it).second = State::PRESS;
						break;

					case State::RELEASE:
						(*it).second = State::NONE;
						break;
					}
				}
				else
				{
					switch ((*it).second)
					{
					case State::NONE:
					case State::RELEASE:
						(*it).second = State::NONE;
						break;

					case State::PRESS:
					case State::ENTER:
						(*it).second = State::RELEASE;
						break;
					}
				}
			}
		}

		std::unordered_map<KeyCode, State> keyState;
	};

}
