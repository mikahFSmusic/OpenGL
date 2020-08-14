#pragma once

#include <vector>
#include <iostream>
#include <functional>

namespace test {
	class Test
	{
	public:
		Test() {}
		// these functions will be overwritten as this class is extended, hence the "virtual" declarations 
		virtual ~Test() {}
		
		// blank body lets you pick which of these functions to override,  rather than instantiating with = 0, which would force them all to be overwritten regardless.
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering Test: " << name << std::endl;

			// TODO: what the actual fuck is a lambda
			// Update: lambdas are weird, look at the documentation, very helpful
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}
	private:
		Test*& m_CurrentTest;
		// makes a vector, which contains a pair, that is a string, and a function
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}