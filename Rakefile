require "bundler/gem_tasks"
require "rake/testtask"

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList['test/**/*_test.rb']
end

require "rake/extensiontask"

task :build => :compile

Rake::ExtensionTask.new("ccmath") do |ext|
  ext.lib_dir = "lib/ccmath"
end

task :compile_and_test do
  Rake::Task['compile'].invoke
  Rake::Task['test'].invoke
end


task default: :compile_and_test
