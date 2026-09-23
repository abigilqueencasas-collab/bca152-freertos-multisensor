#pragma once
enum class DisplayMode { TEMPERATURE, HUMIDITY, LIGHT, MOTION };
DisplayMode nextDisplayMode(DisplayMode m);
DisplayMode previousDisplayMode(DisplayMode m);