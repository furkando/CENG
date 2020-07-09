import xml.sax
import re
from html.parser import HTMLParser

publication_types = ["article", "inproceedings", "proceedings",
                     "book", "incollection", "phdthesis", "mastersthesis", "www"]
field_names = ["author", "editor", "title", "booktitle", "pages", "year", "address", "journal", "volume", "number",
               "month", "url", "ee", "cdrom", "cite", "publisher", "note", "crossref", "isbn", "series", "school", "chapter"]

pub_file = None
field_file = None
publication_count_limit = 1000000
current_publication_index = 0

html_parser = HTMLParser()

class DBLPContentHandler(xml.sax.ContentHandler):
    """
    Reads the dblp.xml file and produces two output files.
          pubFile.txt = (key, pubtype) tuples
          fieldFile.txt = (key, fieldCnt, field, value) tuples
    Each file is tab-separated

    Once the program finishes,  load these two files in a relational database; run createSchema.sql
    """

    def __init__(self):
        xml.sax.ContentHandler.__init__(self)

    def startElement(self, tag, attrs):
        global pub_file, field_file, publication_types, field_names, current_publication_index
        self.content = ""

        if tag == "dblp":
            pub_file = open('pubFile.csv', 'w')
            field_file = open('fieldFile.csv', 'w')

        elif tag in publication_types:
            self.publication_key = attrs["key"]
            self.publication_type = tag
            self.field_count = 0
            current_publication_index += 1

        elif tag in field_names:
            self.field_name = tag

        else:
            pass

    def endElement(self, tag):
        global pub_file, field_file, publication_types, field_names, current_publication_index, publication_count_limit

        if current_publication_index >= publication_count_limit:
            return

        if tag in field_names:
            field_file.write(self.publication_key)
            field_file.write("\t")
            # field_file.write(self.publication_type)
            # field_file.write("\t")
            field_file.write(str(self.field_count))
            field_file.write("\t")
            field_file.write(self.field_name)
            field_file.write("\t")
            
            for c in self.content:
                try:
                    if c == "\\":
                        field_file.write("\\\\")
                    else:
                        field_file.write(c)
                except:
                    field_file.write("?")
                    print(c, end='')

            field_file.write("\n")

            self.field_count += 1

        elif tag in publication_types:
            pub_file.write(self.publication_key)
            pub_file.write("\t")
            pub_file.write(self.publication_type)
            pub_file.write("\n")

        else:
            pass

    def characters(self, content):
        self.content += html_parser.unescape(content)


def main(sourceFileName):
    source = open(sourceFileName)
    xml.sax.parse(source, DBLPContentHandler())


if __name__ == "__main__":
    main("dblp.xml")
