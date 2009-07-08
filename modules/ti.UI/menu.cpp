/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#include <kroll/kroll.h>
#include "ui_module.h"
namespace ti
{
	using std::vector;
	using std::string;

	Menu::Menu() : StaticBoundObject("UI.Menu")
	{
		this->SetMethod("appendItem", &Menu::_AppendItem);
		this->SetMethod("getItemAt", &Menu::_GetItemAt);
		this->SetMethod("insertItemAt", &Menu::_InsertItemAt);
		this->SetMethod("removeItemAt", &Menu::_RemoveItemAt);
		this->SetMethod("getLength", &Menu::_GetLength);
		this->SetMethod("clear", &Menu::_Clear);

		this->SetMethod("addSubmenu", &Menu::_AddSubmenu);
		this->SetMethod("addItem", &Menu::_AddItem);
		this->SetMethod("addSeparatorItem", &Menu::_AddSeparatorItem);
		this->SetMethod("addCheckItem", &Menu::_AddCheckItem);
	}

	Menu::~Menu()
	{
		this->children.clear();
	}

	void Menu::_AddSubmenu(const ValueList& args, SharedValue result)
	{
		UIBinding* binding = UIBinding::GetInstance();
		SharedMenuItem newItem = binding->__CreateMenuItem(args);
		newItem->EnsureHasSubmenu();
		this->AppendItem(newItem);
		result->SetObject(newItem);
	}

	void Menu::_AddItem(const ValueList& args, SharedValue result)
	{
		args.VerifyException("addItem", "?s m|0 s|0");
		UIBinding* binding = UIBinding::GetInstance();

		// Create a menu item object and add it to this item's submenu
		SharedMenuItem newItem = binding->__CreateMenuItem(args);
		this->AppendItem(newItem);
		result->SetObject(newItem);
	}

	void Menu::_AddSeparatorItem(const ValueList& args, SharedValue result)
	{
		UIBinding* binding = UIBinding::GetInstance();
		SharedMenuItem newItem = binding->__CreateSeparatorMenuItem(args);
		this->AppendItem(newItem);
		result->SetObject(newItem);
	}

	void Menu::_AddCheckItem(const ValueList& args, SharedValue result)
	{
		UIBinding* binding = UIBinding::GetInstance();
		SharedMenuItem newItem = binding->__CreateCheckMenuItem(args);
		this->AppendItem(newItem);
		result->SetObject(newItem);
	}

	void Menu::_AppendItem(const ValueList& args, SharedValue result)
	{
		args.VerifyException("appendItem", "o");
		SharedKObject o = args.at(0)->ToObject();

		SharedMenuItem item = o.cast<MenuItem>();
		this->AppendItem(item);
	}

	void Menu::_GetItemAt(const ValueList& args, SharedValue result)
	{
		args.VerifyException("getItemAt", "i");
		size_t index = static_cast<size_t>(args.GetInt(0));
		SharedMenuItem item = this->GetItemAt(index);
		result->SetObject(item);
	}

	void Menu::_InsertItemAt(const ValueList& args, SharedValue result)
	{
		args.VerifyException("insertItemAt", "o,i");
		SharedKObject o = args.at(0)->ToObject();
		SharedMenuItem item = o.cast<MenuItem>();
		size_t index = static_cast<size_t>(args.GetInt(1));

		this->InsertItemAt(item, index);
	}

	void Menu::_RemoveItemAt(const ValueList& args, SharedValue result)
	{
		args.VerifyException("removeItemAt", "i");
		size_t index = static_cast<size_t>(args.GetInt(0));

		this->RemoveItemAt(index);
	}

	void Menu::_GetLength(const ValueList& args, SharedValue result)
	{
		result->SetInt(this->children.size());
	}

	void Menu::_Clear(const ValueList& args, SharedValue result)
	{
		this->children.clear();
		this->ClearImpl();
	}

	void Menu::AppendItem(SharedMenuItem item)
	{
		if (!item.isNull())
		{
			this->children.push_back(item);
			this->AppendItemImpl(item);
		}
	}

	SharedMenuItem Menu::GetItemAt(size_t index)
	{
		if (index >= 0 && index < this->children.size())
		{
			return this->children[index];
		}
		else
		{
			return 0;
		}
	}

	void Menu::InsertItemAt(SharedMenuItem item, size_t index)
	{
		if (item.isNull())
		{
			throw ValueException::FromString("Tried to insert an object that was not a MenuItem");
		}

		if (index >= 0 && index <= this->children.size())
		{
			vector<SharedMenuItem>::iterator i = this->children.begin() + index;
			this->children.insert(i, item);
			this->InsertItemAtImpl(item, index);
		}
		else
		{
			throw ValueException::FromString("Tried to insert a MenuItem at an inavlid index");
		}
	}

	void Menu::RemoveItemAt(size_t index)
	{
		if (index >= 0 && index < this->children.size())
		{
			vector<SharedMenuItem>::iterator i = this->children.begin() + index;
			this->children.erase(i);
			this->RemoveItemAtImpl(index);
		}
		else
		{
			throw ValueException::FromString("Tried to remove a MenuItem at an invalid index");
		}
	}
}