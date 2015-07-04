require 'test_helper'

class CcmathRealTest < Minitest::Test

  def setup
    CCMath.codomain = 'R'
  end

  def assert_infinity(a, *rest)
    rest = ["not infinity: #{a.inspect}"] if rest.empty?
    assert_predicate(a, :infinite?, *rest)
  end

  def assert_nan(a, *rest)
    rest = ["not nan: #{a.inspect}"] if rest.empty?
    assert_predicate(a, :nan?, *rest)
  end

  def assert_float(a, b)
    err = [Float::EPSILON * 4, [a.abs, b.abs].max * Float::EPSILON * 256].max
    assert_in_delta(a, b, err)
  end
  alias check assert_float

  def assert_float_and_int(exp_ary, act_ary)
    flo_exp, int_exp, flo_act, int_act = *exp_ary, *act_ary
    assert_float(flo_exp, flo_act)
    assert_equal(int_exp, int_act)
  end

  def test_atan2
    check(+0.0, CCMath.atan2(+0.0, +0.0))
    check(-0.0, CCMath.atan2(-0.0, +0.0))
    check(+CCMath::PI, CCMath.atan2(+0.0, -0.0))
    check(-CCMath::PI, CCMath.atan2(-0.0, -0.0))

    inf = Float::INFINITY
    expected = 3.0 * CCMath::PI / 4.0
    check(+expected, CCMath.atan2(+inf, -inf))
    check(-expected, CCMath.atan2(-inf, -inf))

    expected = CCMath::PI / 4.0
    check(+expected, CCMath.atan2(+inf, +inf))
    check(-expected, CCMath.atan2(-inf, +inf))

    check(0, CCMath.atan2(0, 1))
    check(CCMath::PI / 4, CCMath.atan2(1, 1))
    check(CCMath::PI / 2, CCMath.atan2(1, 0))
  end

  def test_cos
    check(1.0,  CCMath.cos(0 * CCMath::PI / 4))
    check(1.0 / CCMath.sqrt(2), CCMath.cos(1 * CCMath::PI / 4))
    check(0.0,  CCMath.cos(2 * CCMath::PI / 4))
    check(-1.0, CCMath.cos(4 * CCMath::PI / 4))
    check(0.0,  CCMath.cos(6 * CCMath::PI / 4))
    check(0.5403023058681398,  CCMath.cos(1))
  end

  def test_sin
    check(0.0,  CCMath.sin(0 * CCMath::PI / 4))
    check(1.0 / CCMath.sqrt(2), CCMath.sin(1 * CCMath::PI / 4))
    check(1.0,  CCMath.sin(2 * CCMath::PI / 4))
    check(0.0,  CCMath.sin(4 * CCMath::PI / 4))
    check(-1.0, CCMath.sin(6 * CCMath::PI / 4))
  end

  def test_tan
    check(0.0, CCMath.tan(0 * CCMath::PI / 4))
    check(1.0, CCMath.tan(1 * CCMath::PI / 4))
    assert_operator(CCMath.tan(2 * CCMath::PI / 4).abs, :>, 1024)
    check(0.0, CCMath.tan(4 * CCMath::PI / 4))
    assert_operator(CCMath.tan(6 * CCMath::PI / 4).abs, :>, 1024)
  end

  def test_acos
    check(0 * CCMath::PI / 4, CCMath.acos( 1.0))
    check(1 * CCMath::PI / 4, CCMath.acos( 1.0 / CCMath.sqrt(2)))
    check(2 * CCMath::PI / 4, CCMath.acos( 0.0))
    check(4 * CCMath::PI / 4, CCMath.acos(-1.0))
    assert_raises(CCMath::DomainError) { CCMath.acos(+1.0 + Float::EPSILON) }
    assert_raises(CCMath::DomainError) { CCMath.acos(-1.0 - Float::EPSILON) }
    assert_raises(CCMath::DomainError) { CCMath.acos(2.0) }
  end

  def test_asin
    check( 0 * CCMath::PI / 4, CCMath.asin( 0.0))
    check( 1 * CCMath::PI / 4, CCMath.asin( 1.0 / CCMath.sqrt(2)))
    check( 2 * CCMath::PI / 4, CCMath.asin( 1.0))
    check(-2 * CCMath::PI / 4, CCMath.asin(-1.0))
    assert_raises(CCMath::DomainError) { CCMath.asin(+1.0 + Float::EPSILON) }
    assert_raises(CCMath::DomainError) { CCMath.asin(-1.0 - Float::EPSILON) }
    assert_raises(CCMath::DomainError) { CCMath.asin(2.0) }
  end

  def test_atan
    check( 0 * CCMath::PI / 4, CCMath.atan( 0.0))
    check( 1 * CCMath::PI / 4, CCMath.atan( 1.0))
    check( 2 * CCMath::PI / 4, CCMath.atan(1.0 / 0.0))
    check(-1 * CCMath::PI / 4, CCMath.atan(-1.0))
  end

  def test_cosh
    check(1, CCMath.cosh(0))
    check((CCMath::E ** 1 + CCMath::E ** -1) / 2, CCMath.cosh(1))
    check((CCMath::E ** 2 + CCMath::E ** -2) / 2, CCMath.cosh(2))
  end

  def test_sinh
    check(0, CCMath.sinh(0))
    check((CCMath::E ** 1 - CCMath::E ** -1) / 2, CCMath.sinh(1))
    check((CCMath::E ** 2 - CCMath::E ** -2) / 2, CCMath.sinh(2))
  end

  def test_tanh
    check(CCMath.sinh(0) / CCMath.cosh(0), CCMath.tanh(0))
    check(CCMath.sinh(1) / CCMath.cosh(1), CCMath.tanh(1))
    check(CCMath.sinh(2) / CCMath.cosh(2), CCMath.tanh(2))
  end

  def test_acosh
    check(0, CCMath.acosh(1))
    check(1, CCMath.acosh((CCMath::E ** 1 + CCMath::E ** -1) / 2))
    check(2, CCMath.acosh((CCMath::E ** 2 + CCMath::E ** -2) / 2))
    assert_raises(CCMath::DomainError) { CCMath.acosh(1.0 - Float::EPSILON) }
    assert_raises(CCMath::DomainError) { CCMath.acosh(0) }
  end

  def test_asinh
    check(0, CCMath.asinh(0))
    check(1, CCMath.asinh((CCMath::E ** 1 - CCMath::E ** -1) / 2))
    check(2, CCMath.asinh((CCMath::E ** 2 - CCMath::E ** -2) / 2))
  end

  def test_atanh
    check(0, CCMath.atanh(CCMath.sinh(0) / CCMath.cosh(0)))
    check(1, CCMath.atanh(CCMath.sinh(1) / CCMath.cosh(1)))
    check(2, CCMath.atanh(CCMath.sinh(2) / CCMath.cosh(2)))
    assert_infinity(CCMath.atanh(1))
    assert_infinity(-CCMath.atanh(-1))
    assert_raises(CCMath::DomainError) { CCMath.atanh(+1.0 + Float::EPSILON) }
    assert_raises(CCMath::DomainError) { CCMath.atanh(-1.0 - Float::EPSILON) }
  end

  def test_exp
    check(1, CCMath.exp(0))
    check(CCMath.sqrt(CCMath::E), CCMath.exp(0.5))
    check(CCMath::E, CCMath.exp(1))
    check(CCMath::E ** 2, CCMath.exp(2))
  end

  def test_log
    check(0, CCMath.log(1))
    check(1, CCMath.log(CCMath::E))
    check(0, CCMath.log(1, 10))
    check(1, CCMath.log(10, 10))
    check(2, CCMath.log(100, 10))
    check(CCMath.log(2.0 ** 64), CCMath.log(1 << 64))
    check(CCMath.log(2) * 1024.0, CCMath.log(2 ** 1024))
    assert_infinity(CCMath.log(1.0/0))
    assert_infinity(-CCMath.log(+0.0))
    assert_infinity(-CCMath.log(-0.0))
    assert_raises(CCMath::DomainError) { CCMath.log(-1.0) }
    assert_raises(TypeError) { CCMath.log(1,nil) }
    assert_raises(CCMath::DomainError, '[ruby-core:62309] [ruby-Bug #9797]') { CCMath.log(1.0, -1.0) }
    assert_nan(CCMath.log(0.0, 0.0))
  end

  def test_log2
    check(0, CCMath.log2(1))
    check(1, CCMath.log2(2))
    check(2, CCMath.log2(4))
    check(CCMath.log2(2.0 ** 64), CCMath.log2(1 << 64))
    check(1024.0, CCMath.log2(2 ** 1024))
    assert_infinity(CCMath.log2(1.0/0))
    assert_infinity(-CCMath.log2(+0.0))
    assert_infinity(-CCMath.log2(-0.0))
    assert_raises(CCMath::DomainError) { CCMath.log2(-1.0) }
  end

  def test_log10
    check(0, CCMath.log10(1))
    check(1, CCMath.log10(10))
    check(2, CCMath.log10(100))
    check(CCMath.log10(2.0 ** 64), CCMath.log10(1 << 64))
    check(CCMath.log10(2) * 1024, CCMath.log10(2 ** 1024))
    assert_infinity(CCMath.log10(1.0/0))
    assert_infinity(-CCMath.log10(+0.0))
    assert_infinity(-CCMath.log10(-0.0))
    assert_raises(CCMath::DomainError) { CCMath.log10(-1.0) }
  end

  def test_sqrt
    check(0, CCMath.sqrt(0))
    check(1, CCMath.sqrt(1))
    check(2, CCMath.sqrt(4))
    assert_infinity(CCMath.sqrt(1.0/0))
    assert_equal("0.0", CCMath.sqrt(-0.0).to_s) # insure it is +0.0, not -0.0
    assert_raises(CCMath::DomainError) { CCMath.sqrt(-1.0) }
  end

  def test_cbrt
    check(1, CCMath.cbrt(1))
    check(-2, CCMath.cbrt(-8))
    check(3, CCMath.cbrt(27))
    check(-0.1, CCMath.cbrt(-0.001))
    assert_infinity(CCMath.cbrt(1.0/0))
  end

  def test_frexp
    assert_float_and_int([0.0,  0], CCMath.frexp(0.0))
    assert_float_and_int([0.5,  0], CCMath.frexp(0.5))
    assert_float_and_int([0.5,  1], CCMath.frexp(1.0))
    assert_float_and_int([0.5,  2], CCMath.frexp(2.0))
    assert_float_and_int([0.75, 2], CCMath.frexp(3.0))
  end

  def test_ldexp
    check(0.0, CCMath.ldexp(0.0, 0.0))
    check(0.5, CCMath.ldexp(0.5, 0.0))
    check(1.0, CCMath.ldexp(0.5, 1.0))
    check(2.0, CCMath.ldexp(0.5, 2.0))
    check(3.0, CCMath.ldexp(0.75, 2.0))
  end

  def test_hypot
    check(5, CCMath.hypot(3, 4))
  end

  def test_erf
    check(0, CCMath.erf(0))
    check(1, CCMath.erf(1.0 / 0.0))
  end

  def test_erfc
    check(1, CCMath.erfc(0))
    check(0, CCMath.erfc(1.0 / 0.0))
  end

  def test_gamma
    # sqrt_pi = CCMath.sqrt(CCMath::PI)
    # check(4 * sqrt_pi / 3, CCMath.gamma(-1.5))
    # check(-2 * sqrt_pi, CCMath.gamma(-0.5))
    # check(sqrt_pi, CCMath.gamma(0.5))
    # check(1, CCMath.gamma(1))
    # check(sqrt_pi / 2, CCMath.gamma(1.5))
    # check(1, CCMath.gamma(2))
    # check(3 * sqrt_pi / 4, CCMath.gamma(2.5))
    # check(2, CCMath.gamma(3))
    # check(15 * sqrt_pi / 8, CCMath.gamma(3.5))
    # check(6, CCMath.gamma(4))
    # check(1.1240007277776077e+21, CCMath.gamma(23))
    # check(2.5852016738885062e+22, CCMath.gamma(24))
    #
    # # no SEGV [ruby-core:25257]
    # 31.upto(65) do |i|
    #   i = 1 << i
    #   assert_infinity(CCMath.gamma(i), "CCMath.gamma(#{i}) should be INF")
    #   assert_infinity(CCMath.gamma(i-1), "CCMath.gamma(#{i-1}) should be INF")
    # end
    #
    # assert_raises(CCMath::DomainError) { CCMath.gamma(-Float::INFINITY) }
  end

  def test_lgamma
    # sqrt_pi = CCMath.sqrt(CCMath::PI)
    # assert_float_and_int([CCMath.log(4 * sqrt_pi / 3),  1], CCMath.lgamma(-1.5))
    # assert_float_and_int([CCMath.log(2 * sqrt_pi),     -1], CCMath.lgamma(-0.5))
    # assert_float_and_int([CCMath.log(sqrt_pi),          1], CCMath.lgamma(0.5))
    # assert_float_and_int([0,                          1], CCMath.lgamma(1))
    # assert_float_and_int([CCMath.log(sqrt_pi / 2),      1], CCMath.lgamma(1.5))
    # assert_float_and_int([0,                          1], CCMath.lgamma(2))
    # assert_float_and_int([CCMath.log(3 * sqrt_pi / 4),  1], CCMath.lgamma(2.5))
    # assert_float_and_int([CCMath.log(2),                1], CCMath.lgamma(3))
    # assert_float_and_int([CCMath.log(15 * sqrt_pi / 8), 1], CCMath.lgamma(3.5))
    # assert_float_and_int([CCMath.log(6),                1], CCMath.lgamma(4))
    #
    # assert_raises(CCMath::DomainError) { CCMath.lgamma(-Float::INFINITY) }
  end
end
