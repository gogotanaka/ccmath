# Implement CMath(Ruby-stl) with C.

Ruby has two official libraries related with mathematics.

* **Math** ...Implemented with C. Only for Real number.
* **CMath**...Implemented with Ruby. Accept Complex number.

And now,

* **CCMath**...Implemented with C. Accept Complex number.


```
Calculating -------------------------------------
               CMath    30.828k i/100ms
              CCMath    46.279k i/100ms
-------------------------------------------------
               CMath    443.352k (± 5.5%) i/s -      2.220M
              CCMath    803.920k (± 4.1%) i/s -      4.026M

Comparison:
              CCMath:   803920.3 i/s
               CMath:   443351.8 i/s - 1.81x slower

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

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/[USERNAME]/ccmath. This project is intended to be a safe, welcoming space for collaboration, and contributors are expected to adhere to the [Contributor Covenant](contributor-covenant.org) code of conduct.


## License

The gem is available as open source under the terms of the [MIT License](http://opensource.org/licenses/MIT).
