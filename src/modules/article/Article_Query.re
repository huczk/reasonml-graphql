module FragmentPreview = [%graphql
  {|
    fragment ArticlePreview on Article {
      id
      title
      description
      slug
      favorited
      tagList
      createdAt @bsDecoder(fn: "Gql.decodeDate")
      favoritesCount @bsDecoder(fn: "string_of_int")
      author {
        id
        username
        image
      }
    }
  |}
];

module GetArticles = [%graphql
  {|
    query getArticles($limit: Int, $offset: Int, $author: String, $favorited: String, $tag: String) {
      articles(limit: $limit, offset: $offset, tag: $tag, author: $author, favorited: $favorited) {
        totalPages
        articles {
          ...FragmentPreview.ArticlePreview
        }
      }
    }
  |}
];

module GetFeed = [%graphql
  {|
    query getFeed($limit: Int, $offset: Int) {
      articles: feed(limit: $limit, offset: $offset) {
        totalPages
        articles {
          ...FragmentPreview.ArticlePreview
        }
      }
    }
  |}
];

module GetArticle = [%graphql
  {|
    query getArticle($slug: String!) {
      article(slug: $slug) {
        ...FragmentPreview.ArticlePreview @bsField(name: "preview")
        body
        author {
          id
          image
          username
          following
        }
      }
    }
  |}
];

module AddArticle = [%graphql
  {|
    mutation addArticle($title: String!, $description: String!, $body: String!, $tagList: [String!] = []) {
      addArticle(title: $title, description: $description, body: $body, tagList: $tagList) {
        ...FragmentPreview.ArticlePreview
      }
    }
  |}
];

module UpdateArticle = [%graphql
  {|
    mutation updateArticle($slug: String!, $title: String!, $description: String!, $body: String!) {
      updateArticle(slug: $slug, title: $title, description: $description, body: $body) {
        ...FragmentPreview.ArticlePreview
      }
    }
  |}
];

module Delete = [%graphql
  {|
    mutation deleteArticle($slug: String!) {
      deleteArticle(slug: $slug) {
        success
      }
    }
  |}
];

module Favorite = [%graphql
  {|
    mutation favorite($slug: String!, $favorited: Boolean!) {
      favorite(slug: $slug, favorite: $favorited) {
        id
        favorited
        favoritesCount
      }
    }
  |}
];