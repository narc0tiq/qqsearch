QQSearch is... hard to define. It's a search engine (OpenSearch compatible!)
which uses other search engines. But it also serves as a convenient bookmark
keeper, if the search assist functionality isn't used.

Mostly, it works like a private version of [yubnub](http://yubnub.org/), meant
for people who want to keep their (search) bookmarks private, and/or to keep a
historical record of their searches (the author notes, as of 2011-06-22, he
had 5488 search queries archived since 2009-07-13).


The magic behind using QQSearch as a search engine is mostly as simple as
knowing how to form a search URL. When adding a URL mapping to QQSearch, one can
use placeholders to be filled in on later use (see also "help add" in your new
install of QQSearch).


For example, a Google search URL looks like this:

    http://google.com/search?q=search+terms+go+here

Because you know where the search terms go, you can add a google search to your
QQSearch using \{\{\*\}\} as the placeholder for them:

    add google http://google.com/search?q={{*}}

Now, when you type

    google you're a good man charlie brown

into QQSearch, it knows to translate that to

    http://google.com/search?q=you're+a+good+man+charlie+brown

and sends you straight there after translation (storing the query in its search
history while it's doing that).


Now, QQSearch doesn't really care if it's sending you to a search URL or any
other kind of URL. A URL is a URL is a URL, ad infinitum. So, you can do some
really clever things. For instance, have you noticed that twitter URLs generally
look like this:

    http://twitter.com/username

You could therefore give yourself a shortcut to practically any Twitter user, as
long as you knew their @name:

    add t http://twitter.com/{{1}}

And you could use that to quickly jump to, for instance, @notch's twitter:

    t notch

Which immediately gets translated to:

    http://twitter.com/notch


The demo site at http://narc.ro/demos/qqsearch/ has a few examples of URL
mappings that the author has found useful, and if you look for the patterns,
you can make your own. And by hosting QQSearch on an Internet-facing machine
(though not an insecure one!), you can have the same bookmarks and search
shortcuts from any computer connected to the Internet -- and aggregate the
search history from all of them in one place!
