#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>
#include "Datastructures.h"

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
		DirectX::XMFLOAT4 cameraPos;
		DirectX::XMFLOAT4 cameraDir;
		Camera* camera;

		bool	pressFrame[I_KEY::COUNT];
		bool	releaseFrame[I_KEY::COUNT];
		bool	currentFrame[I_KEY::COUNT];
		bool	previousFrame[I_KEY::COUNT];
		int		currentMousePos[2];
		int		previousMousePos[2];

		float cameraSpeed;

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
		bool	KeyDown( I_KEY::Keys key_ );
		bool	KeyPressed( I_KEY::Keys key_ );
		bool	KeyReleased( I_KEY::Keys key_ );
	private:
};
#endif