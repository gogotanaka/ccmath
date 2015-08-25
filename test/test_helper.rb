$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'ccmath'

require 'minitest/autorun'


class ::Complex
  def i
    self * 1i
  end
end

1.i
#=> (0+1i)

(1+0i).i
#=> NoMethodError: undefined method `i' for (1+0i):Complex
