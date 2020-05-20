







float Controller::NEAR(floatdist_near) {
  if (dist_near <= 0.25) {
    return (1.0);
  }
  else if (dist_near >= 0.75) {
    return (0.0);
  }
  else {
    return ((-2.0 * dist_near) + 1.5);
  }

}
float Controllers::FAR(floatdist_far) {
  if (dist_far <= 0.25) {
    return (0.0);
  }
  else if (dist_far >= 0.75) {
    return (1.0);
  }
  else {
    return ((2.0 * dist_far)–0.5);
  }
}

float DEFUZZIFY_SPEED(void) {
  floatspeed;
  speed(max_SLOW * 0.0)   + (max_MED * 25.0)  + (max_FAST * 50.0);
  return (speed);
}
// Helper functions

float MIN(floatmin1, floatmin2, floatmin3) {
  floatmin_val;
  min_val = min1;
  if (min2 < min_val) {
    min_val = min2;
  }
  if (min3 < min_val) {
    min_val = min3;
  }
  return (min_val);
}

floatMAX(floatmax1, floatmax2, floatmax3) {
  floatmax_val;
  max_val = max1;
  if (max2 > max_val) {
    max_val = max2;
  }
  if (max3 > max_val) {
    max_val = max3;
  }
  return (max_val);
}
