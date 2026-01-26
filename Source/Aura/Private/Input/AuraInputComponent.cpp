// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputComponent.h"

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityAction(const UAuraInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	for (const auto& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (ReleasedFunc.IsValid())
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			
			if (PressedFunc.IsValid())
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			
			if (HeldFunc.IsValid())
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object,HeldFunc, Action.InputTag);
			}
		}
	}
}
