#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>

#define Input_KeyDown( key ) Input::GetInstance()->KeyDown( key )
#define Input_KeyPressed( key ) Input::GetInstance()->KeyPressed( key )
#define Input_KeyReleased( key ) Input::GetInstance()->KeyReleased( key )

namespace I_KEY
{
	enum Keys
	{
		A,
		D,
		H,
		J,
		S,
		W,
		ARROW_LEFT,
		ARROW_RIGHT,
		ARROW_DOWN,
		ARROW_UP,
		MOUSE_LEFT,
		MOUSE_RIGHT,
		COUNT
	};
}

class Input
{
	//Variables
	public:
		bool pressFrame[I_KEY::COUNT];
		bool releaseFrame[I_KEY::COUNT];
		bool currentFrame[I_KEY::COUNT];
		bool previousFrame[I_KEY::COUNT];
		int mousePos[2];

	private:

	//Functions
	public:
		HRESULT	Initialize();
		void	Release();

		static Input* GetInstance()
		{
			static Input* instance = new Input();
			return instance;
		}

				Input( Input const& )		= delete;
		void	operator=( Input const& )	= delete;

	private:
		Input() {}

	public:
		void	Update();
		void	Clear();
		bool	KeyDown( I_KEY::Keys key );
		bool	KeyPressed( I_KEY::Keys key );
		bool	KeyReleased( I_KEY::Keys key );
	private:
};
#endif