open ReactUtils;

type t =
  | Global
  | Feed
  | Author(string)
  | Favorited(string)
  | Tag(string);

[@react.component]
let make = (~listType) => {
  let {UserProvider.user} = UserProvider.useUser();
  let (page, setPage) = React.useState(() => 1);
  let offset = page - 1;

  let isFeed =
    switch (listType, user) {
    | (Feed, LoggedIn(_)) => true
    | _ => false
    };

  let (simpleArticles, _full) =
    ApolloHooks.useQuery(
      ~skip=isFeed,
      ~variables={
        let variables = Article_Query.GetArticles.makeVariables(~offset);

        switch (listType) {
        | Tag(tag) => variables(~tag, ())
        | Author(author) => variables(~author, ())
        | Favorited(favorited) => variables(~favorited, ())
        | _ => variables()
        };
      },
      Article_Query.GetArticles.definition,
    );

  let (simpleFeed, _full) =
    ApolloHooks.useQuery(
      ~skip=!isFeed,
      ~variables=Article_Query.GetFeed.makeVariables(~offset, ()),
      Article_Query.GetFeed.definition,
    );

  <div>
    {switch (isFeed ? simpleFeed : simpleArticles) {
     | Loading =>
       <div className="article-preview"> "Loading articles..."->s </div>
     | Data(data) =>
       switch (data##articles##articles) {
       | [||] =>
         <div className="article-preview">
           "No articles are here... yet."->s
         </div>
       | _ =>
         <>
           {data##articles##articles
            ->Belt.Array.map(
                fun
                | None => React.null
                | Some(article) => <ArticlePreview key=article##id article />,
              )
            |> React.array}
           <Pagination page setPage pages=data##articles##totalPages />
         </>
       }
     | _ => React.null
     }}
  </div>;
};