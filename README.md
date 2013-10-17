# URL Parser
A Drupal 7 module that enabled site administrators to define key defitions 
for URL parameters and site users to get explanations for keys and values 
in a URL.

## Quick start
1. Clone repository into Drupal module folder.
2. Enable module in Drupal web interface or using Drush.
3. Configure on http://example.com/admin/url_parser. See Configuration below.
4. Use on http://example.com/url_parser. See Usage below.

## Demo
No demo yet.

## Bug and feature requests
Use [issue tracker](https://github.com/troelsselch/urlparser/issues) on github.

## Functionality overview.
- Add keys by host.
- Delete keys.
- Gather data for keys - known and unknown.
- Enter URL to be parsed.
- Load keys by configuration file.
- Configure access to usage and configuration of URL Parser.

## Documentation
This is currently the only documentation there is. If needed in the 
future I will create a web page for this project.

### Configuration
Assuming the module is installed on http://example.com

#### View defined keys and data for keys
1. Go to http://example.com/admin/config/url_parser
2. See the keys defined under the heading *Defined keys* and data 
for keys defined under the heading *Data for keys* (after the form).

Simple as that. However when you first install the module there will 
be no keys defined.

#### Add new key
1. Go to http://example.com/admin/config/url_parser
2. Find the form with the heading *Add new key*.
3. Fill the form. For example:
  * Host: `www.google.com`
  * Key: `q`
  * Key description: `Your search query.`
4. Click `Add key definition`.

Your key is now added and should be shown in the table.

#### Upload file with key definitions
1. Go to http://testingdrupal.local/admin/config/url_parser/load_config.
2. Select a file with the file selector.
3. Click `Parse`.

Your file will now be uploaded and parsed. If any keys have already 
been defined for a host they will **not** be overridden. To replace a 
key you must delete it and define it/upload a file.

### Usage
This module must be enabled on a site by a site admin. Here we assume that it has been installed on http://example.com.

1. Go to http://example.com/url_parser.
2. Paste a URL into the text field. For example `https://www.google.com/search?client=ubuntu&channel=fs&q=clean+all+the+things&ie=utf-8&oe=utf-8`.
3. Click `Parse`.

When the page reloads you should see an explanation of the keys and/or a list of keys that are unknown.

## Todo
### Version 1
- Document code.
- Write help functions.
  - https://drupal.org/documentation/modules/help
- Use rendering functions for admin_page and search result. Other candidates?
- Rename function names.
- Review variable names.
- Link to github project in google.ini
- Setup demo.

### Later versions
- Add handler for if none of the keys are known
- Create `$_SERVER['HTTP_REFERER']` catch (drupal hook?) module that uses this parser. Can be used for statistics.
- Log on failed deletion.
- Allow description to use $key.
- Maybe: Test for ctools and provide delete confirm in overlay and form submit ajax.
- Consider extension to include parsing parameters from the links on the search result page.
- Browser keys (non admin).
- Find a way to include Google Advanced search tools parameters (See examples/google.ini).
- Improve/Elaborate on Google explanations.
- Make url_parser_host in url_parser_admin_form suggest known values.
- When adding new keys by file check to see if any keys for host are already defined. Make it optional what to do: override or keep existing.

# Project Information
## Project hosting
https://github.com/troelsselch/urlparser

## Contributers
- Troels Selch ([@troelsselch](https://twitter.com/troelsselch))

## Copyright/License
[MIT License](./LICENSE)

