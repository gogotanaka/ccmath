$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'ccmath'

require 'minitest/autorun'


class ::Complex
  def i
    self * 1i
  end
end
