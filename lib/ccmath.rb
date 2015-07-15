require "ccmath/version"
require "ccmath/ccmath"

module CCMath
  class << self
    %w[
      atan2
      cbrt
      frexp
      ldexp
      hypot
      erf
      erfc
      lgamma
    ].each do |meth|
      define_method(meth) do |*args, &blk|
        Math.send(meth, *args, &blk)
      end
    end
  end
end
