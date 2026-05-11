#pragma once
class Input
{
public:
	Input() = default;
	Input(const Input&) = delete;
	~Input() = default;

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};