## Hopefully, CCMath is better than CMath...! (・ω・;)

[![Build Status](https://travis-ci.org/gogotanaka/ccmath.svg?branch=master)](https://travis-ci.org/gogotanaka/ccmath)

CCMath is CMath implemented with C-lang.

## Demo
```ruby
CCMath.cos(1i)
#=> (1.5430806348152437+0.0i)

CCMath.cos(1)
#=> (0.5403023058681398+0.0i)
```

## Bench

```
Calculating -------------------------------------
               CMath    28.423k i/100ms
              CCMath    61.123k i/100ms
-------------------------------------------------
               CMath    379.852k (± 4.2%) i/s -      1.904M
              CCMath      1.163M (± 4.0%) i/s -      5.807M

Comparison:
              CCMath:  1163020.7 i/s
               CMath:   379852.3 i/s - 3.06x slower

```

## What's the difference between CCMath and CMath..?(・ω・;)
1. CCMath return Complex object even if image part is equivalent 0.
2. Faster(about 3 times)
3. Better error handling

Other behaviours suppose to be same.

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

# Contributor Code of Conduct

As contributors and maintainers of this project, we pledge to respect all people who contribute through reporting issues, posting feature requests, updating documentation, submitting pull requests or patches, and other activities.

We are committed to making participation in this project a harassment-free experience for everyone, regardless of level of experience, gender, gender identity and expression, sexual orientation, disability, personal appearance, body size, race, ethnicity, age, or religion.

Examples of unacceptable behavior by participants include the use of sexual language or imagery, derogatory comments or personal attacks, trolling, public or private harassment, insults, or other unprofessional conduct.

Project maintainers have the right and responsibility to remove, edit, or reject comments, commits, code, wiki edits, issues, and other contributions that are not aligned to this Code of Conduct. Project maintainers who do not follow the Code of Conduct may be removed from the project team.

Instances of abusive, harassing, or otherwise unacceptable behavior may be reported by opening an issue or contacting one or more of the project maintainers.

This Code of Conduct is adapted from the [Contributor Covenant](http://contributor-covenant.org), version 1.0.0, available at [http://contributor-covenant.org/version/1/0/0/](http://contributor-covenant.org/version/1/0/0/)
