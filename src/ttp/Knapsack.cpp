#include "Knapsack.hpp"

#include <algorithm>
#include <numeric>

namespace ttp {

Knapsack::Knapsack(const uint32_t capacity)
	: capacity(capacity)
	, currentWeight(0)
{
}

const ItemsPerCity & Knapsack::getItemsPerCity() const
{
	return itemsPerCity;
}

uint32_t Knapsack::getWeightForCity(const uint32_t cityId) const
{
	if (itemsPerCity.find(cityId) == itemsPerCity.end())
		return 0u;
	const auto& items = itemsPerCity.at(cityId);
	return std::accumulate(items.cbegin(), items.cend(), 0, [](const auto& acc, const auto& item) {return acc + item.weight; });
}

void Knapsack::fillKnapsack(const TtpConfig& ttpConfig)
{
	// based on profit to weight ratio
	auto allItems = ttpConfig.items;
	std::sort(allItems.begin(), allItems.end(),
		[](const auto& lhs, const auto& rhs)
		{
			return lhs.profit / lhs.weight < rhs.profit / rhs.weight;
		});
	for (auto rit = allItems.crbegin(); rit != allItems.crend() && currentWeight < capacity; rit++)
	{
		auto& item = *rit;
		if (currentWeight + item.weight <= capacity)
		{
			currentWeight += item.weight;
			itemsPerCity[item.cityId].push_back(std::move(item));
		}
	}
	knapsackValue =
		std::accumulate(itemsPerCity.cbegin(), itemsPerCity.cend(), 0,
			[](const auto& acc, const auto& pair)
			{
				return acc + std::accumulate(pair.second.cbegin(), pair.second.cend(), 0,
					[](const auto& acc, const auto& item) {return acc + item.profit; });
			}
		);
}

int32_t Knapsack::getKnapsackValue() const
{
	return knapsackValue;
}

} // namespace ttp