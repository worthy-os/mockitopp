#include <boost/regex.hpp>
#include <gtest/gtest.h>
#include <mockitopp/MockObject.hpp>
#include <mockitopp/matchers/optional/Regex.hpp>

using mockitopp::MockObject;
using mockitopp::matcher::regex;

struct regex_interface
{
   virtual std::string const_char_s(const char*) = 0;
   virtual std::string std_string(const std::string&) = 0;
   virtual std::string composite(const char*, const std::string&) = 0;
};

TEST(test_regex, const_char)
{
   MockObject<regex_interface> mock;
   mock(&regex_interface::const_char_s).when(regex(".*foo.*")).thenReturn("matches__.*foo.*");
   mock(&regex_interface::const_char_s).when(regex(".*bar.*")).thenReturn("matches__.*bar.*");
   mock(&regex_interface::const_char_s).when(regex("[A-Z]*")).thenReturn("matches__[A-Z]*");
   regex_interface& obj = mock.getInstance();

   ASSERT_EQ("matches__.*foo.*", obj.const_char_s("foo"));
   ASSERT_EQ("matches__.*bar.*", obj.const_char_s("bar"));
   ASSERT_EQ("matches__[A-Z]*",  obj.const_char_s("ABCDEFGHIKLMNOPQRSTUVWXYZ"));
   ASSERT_THROW(obj.const_char_s("FOO-BAR"),    mockitopp::detail::IncompleteImplementationException);
   ASSERT_THROW(obj.const_char_s("1234567890"), mockitopp::detail::IncompleteImplementationException);
}

TEST(test_regex, std_string)
{
   MockObject<regex_interface> mock;
   mock(&regex_interface::std_string).when(regex(std::string(".*foo.*"))).thenReturn("matches__.*foo.*");
   mock(&regex_interface::std_string).when(regex(std::string(".*bar.*"))).thenReturn("matches__.*bar.*");
   mock(&regex_interface::std_string).when(regex(std::string("[A-Z]*"))).thenReturn("matches__[A-Z]*");
   regex_interface& obj = mock.getInstance();

   ASSERT_EQ("matches__.*foo.*", obj.std_string("foo"));
   ASSERT_EQ("matches__.*bar.*", obj.std_string("bar"));
   ASSERT_EQ("matches__[A-Z]*",  obj.std_string("ABCDEFGHIKLMNOPQRSTUVWXYZ"));
   ASSERT_THROW(obj.std_string("FOO-BAR"),    mockitopp::detail::IncompleteImplementationException);
   ASSERT_THROW(obj.std_string("1234567890"), mockitopp::detail::IncompleteImplementationException);
}

TEST(test_regex, composite)
{
   MockObject<regex_interface> mock;
   mock(&regex_interface::composite).when(regex(".*foo.*"), regex(std::string(".*bar.*"))).thenReturn("matches__.*foo.*__.*bar.*");
   mock(&regex_interface::composite).when(regex(".*FOO.*"), regex(std::string(".*BAR.*"))).thenReturn("matches__.*FOO.*__.*BAR.*");
   mock(&regex_interface::composite).when(regex("[a-z]*"),  regex(std::string("[A-Z]*"))).thenReturn("matches__[a-z]*__[A-Z]*");
   regex_interface& obj = mock.getInstance();

   ASSERT_EQ("matches__.*foo.*__.*bar.*", obj.composite("foo", "bar"));
   ASSERT_EQ("matches__.*foo.*__.*bar.*", obj.composite("abc-foo-xyz", "123-bar-890"));
   ASSERT_EQ("matches__.*FOO.*__.*BAR.*", obj.composite("FOO", "BAR"));
   ASSERT_EQ("matches__.*FOO.*__.*BAR.*", obj.composite("FOOTLONG SANDWICH", "BARLEY SOUP"));
   ASSERT_EQ("matches__[a-z]*__[A-Z]*",   obj.composite("abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIKLMNOPQRSTUVWXYZ"));
   ASSERT_THROW(obj.composite("foo", "abcdefghij"), mockitopp::detail::IncompleteImplementationException);
   ASSERT_THROW(obj.composite("1234567890", "bar"), mockitopp::detail::IncompleteImplementationException);
}
