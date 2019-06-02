/* This file is part of nSkinz by namazso, licensed under the MIT license:
*
* MIT License
*
* Copyright (c) namazso 2018
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#pragma once
#include "item_definitions.hpp"
#include "kit_parser.hpp"

#include <limits>
#include <unordered_map>
#include <array>
#include <algorithm>

#undef min

template<typename Container, typename T1, typename T2, typename TC>
class value_syncer
{
	using container_type = typename Container::value_type;

	const Container& container;
	T1& key;
	T2& value;
	const TC container_type::* member;

public:
	value_syncer(const Container& container, T1& key, T2& value, const TC container_type::* member)
		: container{container}
		, key{key}
		, value{value}
		, member{member}
	{}

	auto key_to_value() const -> void
	{
		key = std::clamp(key, T1(0), T1(container.size() - 1));
		value = container.at(key).*member;
	}

	auto value_to_key() const -> void
	{
		auto it = std::find_if(std::begin(container), std::end(container), [this](const container_type& x)
		{
			return value == x.*member;
		});

		// Originally I wanted this to work with maps too, but fuck that
		if(it != std::end(container))
			key = it - std::begin(container);
		else
			key = T1(0);
	}
};

template<typename Container, typename T1, typename T2, typename TC>
static auto do_sync(const Container& container, T1& key, T2& value, TC Container::value_type::* member) -> void
{
	auto syncer = value_syncer<Container, T1, T2, TC>{ container, key, value, member };
	syncer.key_to_value();
}

struct sticker_setting
{
	void update()
	{
		do_sync(game_data::sticker_kits, kit_vector_index, kit, &game_data::PaintKit::id);
	}

	int kit = 0;
	int kit_vector_index = 0;
	float wear = std::numeric_limits<float>::min();
	float scale = 1.f;
	float rotation = 0.f;
};

struct item_setting
{
	void update()
	{
		do_sync(
			game_data::weapon_names,
			definition_vector_index,
			definition_index,
			&game_data::weapon_name::definition_index
		);

		do_sync(
			game_data::quality_names,
			entity_quality_vector_index,
			entity_quality_index,
			&game_data::quality_name::index
		);

		const std::vector<game_data::PaintKit>* kit_names;
		const std::vector<game_data::weapon_name>* defindex_names;

		if (definition_index == GLOVE_T_SIDE)
		{
			kit_names = &game_data::glove_kits;
			defindex_names = &game_data::glove_names;
		}
		else
		{
			kit_names = &game_data::skin_kits;
			defindex_names = &game_data::knife_names;
		}

		do_sync(
			*kit_names,
			paint_kit_vector_index,
			paint_kit_index,
			&game_data::PaintKit::id
		);

		do_sync(
			*defindex_names,
			definition_override_vector_index,
			definition_override_index,
			&game_data::weapon_name::definition_index
		);

		for(auto& sticker : stickers)
			sticker.update();
	}

	char name[32] = "Default";
	bool enabled = false;
	int definition_vector_index = 0;
	int definition_index = 1;
	int entity_quality_vector_index = 0;
	int entity_quality_index = 0;
	int paint_kit_vector_index = 0;
	int paint_kit_index = 0;
	int definition_override_vector_index = 0;
	int definition_override_index = 0;
	int seed = 0;
	int stat_trak = 0;
	float wear = std::numeric_limits<float>::min();
	char custom_name[32] = "";
	std::array<sticker_setting, 5> stickers;
};

class config_ {
public:
    item_setting* get_by_definition_index(int definition_index);
};

extern config_ g_config;