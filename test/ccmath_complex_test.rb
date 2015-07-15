require 'test_helper'

class CcmathComplexTest < Minitest::Test

  def setup
    CCMath.set = 'C'
  end

  def test_sqrt
    assert_equal CCMath.sqrt(1i), CCMath.sqrt(1.0i), '[ruby-core:31672]'
    assert_equal Complex(0,2), CCMath.sqrt(-4.0)
    assert_equal Complex(0,2), CCMath.sqrt(-4)
    assert_equal Complex(0,2), CCMath.sqrt(Rational(-4))
    assert_equal Complex(0,3), CCMath.sqrt(-9.0)
    assert_equal Complex(0,3), CCMath.sqrt(-9)
    assert_equal Complex(0,3), CCMath.sqrt(Rational(-9))
    assert_in_delta (1.272019649514069+0.7861513777574233i), CCMath.sqrt(1+2i)
    assert_in_delta 3.0i, CCMath.sqrt(-9)
  end

  def test_log
    assert_in_delta (0.8047189562170503+1.1071487177940904i), CCMath.log(1+2i)
    assert_in_delta (0.7324867603589635+1.0077701926457874i), CCMath.log(1+2i,3)
    assert_in_delta Math::PI*1i                             , CCMath.log(-1)
    assert_in_delta 3.0                                     , CCMath.log(8, 2)
    assert_in_delta (1.092840647090816-0.42078724841586035i), CCMath.log(-8, -2)
  end

  def test_trigonometric_functions
    assert_in_delta CCMath.sinh(2).i, CCMath.sin(2i)
    assert_in_delta CCMath.cosh(2),   CCMath.cos(2i)
    assert_in_delta CCMath.tanh(2).i, CCMath.tan(2i)

    assert_in_delta CCMath.sin(2).i, CCMath.sinh(2i)
    assert_in_delta CCMath.cos(2),   CCMath.cosh(2i)
    assert_in_delta CCMath.tan(2).i, CCMath.tanh(2i)

    assert_in_delta 1+1i, CCMath.sin(CCMath.asin(1+1i))
    assert_in_delta 1+1i, CCMath.cos(CCMath.acos(1+1i))
    assert_in_delta 1+1i, CCMath.tan(CCMath.atan(1+1i))

    assert_in_delta 1+1i, CCMath.sinh(CCMath.asinh(1+1i))
    assert_in_delta 1+1i, CCMath.cosh(CCMath.acosh(1+1i))
    assert_in_delta 1+1i, CCMath.tanh(CCMath.atanh(1+1i))

    assert_in_delta (3.165778513216168+1.959601041421606i),     CCMath.sin(1+2i)
    assert_in_delta (2.0327230070196656-3.0518977991517997i),   CCMath.cos(1+2i)
    assert_in_delta (0.033812826079896774+1.0147936161466338i), CCMath.tan(1+2i)
    assert_in_delta (-0.4890562590412937+1.4031192506220405i),  CCMath.sinh(1+2i)
    assert_in_delta (-0.64214812471552+1.0686074213827783i),    CCMath.cosh(1+2i)
    assert_in_delta (1.16673625724092-0.2434582011857252i),     CCMath.tanh(1+2i)
    assert_in_delta (0.4270785863924755+1.5285709194809978i),   CCMath.asin(1+2i)
    assert_in_delta (1.1437177404024204-1.528570919480998i),    CCMath.acos(1+2i)
    assert_in_delta (1.3389725222944935+0.4023594781085251i),   CCMath.atan(1+2i)
    assert_in_delta (1.4693517443681852+1.0634400235777521i),   CCMath.asinh(1+2i)
    assert_in_delta (1.528570919480998+1.1437177404024204i),    CCMath.acosh(1+2i)
    assert_in_delta (0.17328679513998635+1.1780972450961724i),  CCMath.atanh(1+2i)
  end

  def test_functions
    assert_in_delta (-1.1312043837568135+2.4717266720048188i), CCMath.exp(1+2i)
    assert_in_delta (-1),                                      CCMath.exp(Math::PI.i)
    assert_in_delta (1.1609640474436813+1.5972779646881088i),  CCMath.log2(1+2i)
    assert_in_delta (0.3494850021680094+0.480828578784234i),   CCMath.log10(1+2i)
    # assert_in_delta (1.3389725222944935+0.4023594781085251i),  CCMath.atan2(1+2i,1)
  end

  def test_error_handling
    # assert_raises(TypeError, "Numeric Number required") { CCMath.acos("2") }
    # assert_raises(TypeError, "Numeric Number required") { CCMath.log("2") }
    # assert_raises(ArgumentError) { CCMath.log(2, "2") }
    assert_raises(TypeError, "Numeric Number required") { CCMath.log(2, "2") }
    # assert_raises(NoMethodError) { CCMath.log(2, 2i) }
    assert_raises(CCMath::DomainError) { CCMath.log(2, 2i) }
    # assert_raises(RangeError) { CCMath.hypot(2i, 2i) }
    # assert_raises(CCMath::DomainError) { CCMath.hypot(2i, 2i) }
  end

  def test_cbrt_returns_principal_value_of_cube_root
    # assert_equal (-8)**(1.0/3), CCMath.cbrt(-8), '#3676'
  end
end
