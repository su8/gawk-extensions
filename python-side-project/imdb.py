#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Created by Aaron Caffrey
# License: GPLv3
from re import sub
try:
    from urllib.request import urlopen
except ImportError:
    from urllib import urlopen

class IMDB(object):
    def __init__(self, given_link):
        self._link = given_link
        self._d = {'attrs': {
        'metascore': list(), 'plot': list(), 'casts': list(), 'genre': list(), 
        'director': list(), 'year': list(), 'rating': list(), 'runtime': list(), 
        'poster': list(), 'country': list(), 'title': list(), 'release_date': list()},
        'check': {
        'tt_ov_dr': 'director', 'Metascore:': 'metascore', 
        '''tt_ov_st" itemprop='url'><span class="itemprop"''': 'casts',
        '<strong><span itemprop="ratingValue">': 'rating',
        'See all release dates': 'release_date', '/country/': 'country',
        '<span class="itemprop" itemprop="genre">': 'genre',
        'src="http://ia.media-imdb.com/images/M/': 'poster',
        '<h4 class="inline">Runtime:</h4>': 'runtime', 
        '<p itemprop="description">': 'plot'}}

    def __getattr__(self, requested_attr):
        return str(' '.join(self._d['attrs'][requested_attr]))

    def parse(self):
        imdb_list = urlopen(self._link).read().decode('utf-8').split("\n")
        def append_to_dict(key, str_to_strip):
            self._d['attrs'][key].append(' '.join(strip_html(str_to_strip)
                .replace('|', '').replace('src=', '').replace('"', '').split()))
        def strip_html(val):
            return sub('<[^>]*>', '', val)

        for x in imdb_list:
            for key,value in self._d['check'].items():
                if key in x:
                    (append_to_dict(value, x) if not 
                        value in ['runtime', 'plot'] else self._d['attrs'][value]\
                        .append(' '.join(strip_html(
                    imdb_list[imdb_list.index(x) + 1])\
                    .replace('See full summary&nbsp;&raquo;', '').split())))
            if 'class="header"' in x:
                self._d['attrs']['title'].append(strip_html(x).lstrip().rstrip())
                year = ' '.join(strip_html(
                    imdb_list[imdb_list.index(x) + 1]).replace('(', '')\
                    .replace(')', '').split())
                self._d['attrs']['year'].append(year)