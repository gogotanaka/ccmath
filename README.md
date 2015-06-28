# Implement CMath(Ruby-stl) with C.

Ruby has two official libraries related with mathematics.

* **Math** ...Implemented with C. Only for Real number.
* **CMath**...Implemented with Ruby. Accept Complex number.

And now,

* **CCMath**...Implemented with C. Accept Complex number.


```
Calculating -------------------------------------
               CMath    31.262k i/100ms
              CCMath    53.484k i/100ms
-------------------------------------------------
               CMath    437.150k (± 7.1%) i/s -      2.188M
              CCMath      1.044M (±28.3%) i/s -      4.600M

Comparison:
              CCMath:  1044410.2 i/s
               CMath:   437150.3 i/s - 2.39x slower

```

## Installation


```ruby
gem 'ccmath'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install ccmath

## Development

After checking out the repo, run `bin/setup` to install dependencies.

Then, run `bundle exec rake compile` to compile library and `bundle exec rake test` to run the tests.

You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`.

<!-- To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org). -->

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/[USERNAME]/ccmath. This project is intended to be a safe, welcoming space for collaboration, and contributors are expected to adhere to the [Contributor Covenant](contributor-covenant.org) code of conduct.


## License

The MIT License (MIT)

Copyright (c) 2015 gogotanaka

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
