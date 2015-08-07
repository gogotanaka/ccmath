require "ccmath/version"
require "ccmath/ccmath"

module CCMath
  def cbrt(z)
    z ** (1.0/3)
  end

  def atan2(y,x)
    begin
      if y.real? and x.real?
        atan2!(y,x)
      else
        (-1.0).i * log((x + 1.0.i * y) / sqrt(x * x + y * y))
      end
    rescue NoMethodError
      handle_no_method_error
    end
  end

  %w[
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

  module_function :cbrt
  module_function :atan2
  module_function :frexp
  module_function :ldexp
  module_function :hypot
  module_function :erf
  module_function :erfc
  module_function :lgamma
end
