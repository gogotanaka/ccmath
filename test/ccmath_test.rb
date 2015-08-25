require 'test_helper'

class CCMathTest < Minitest::Test
  def test_error_handling
    assert_raises(TypeError, "Numeric Number required") { CMath.acos("2") }
    assert_raises(TypeError, "Numeric Number required") { CMath.log("2") }
    assert_raises(TypeError, "Numeric Number required") { CMath.log(2, "2") }
    assert_raises(CCMath::DomainError) { CMath.log(2, 2i) }
  end
end
