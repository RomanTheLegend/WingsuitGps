#pragma once

#include "../common.h"

class GpsInterface
{
public:
  virtual void refresh() = 0;
  virtual DataPoint getLatestData() = 0;
};

