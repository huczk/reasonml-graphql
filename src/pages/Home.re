open ReactUtils;

type tabs =
  | Feed
  | Global
  | Tag(string);

let listType =
  fun
  | Feed => ArticleList.Feed
  | Global => ArticleList.Global
  | Tag(tag) => ArticleList.Tag(tag);

[@react.component]
let make = () => {
  let {UserProvider.user} = UserProvider.useUser();
  let isLogged = user !== Anonymous;
  let (tab, setTab) = React.useState(() => isLogged ? Feed : Global);
  let tag =
    switch (tab) {
    | Tag(tag) => tag
    | _ => ""
    };

  <div className="home-page">
    <div className="banner">
      <div className="container">
        <h1 className="logo-font"> "conduit"->s </h1>
        <p> "A place to share your knowledge."->s </p>
      </div>
    </div>
    <div className="container page">
      <div className="row">
        <div className="col-md-9">
          <div className="feed-toggle">
            <ul className="nav nav-pills outline-active">
              {isLogged
                 ? <TabItem
                     text="Your Feed"
                     active={tab == Feed}
                     onClick={eventPrevent(_ => setTab(_ => Feed))}
                   />
                 : React.null}
              <TabItem
                text="Global Feed"
                active={tab == Global}
                onClick={eventPrevent(_ => setTab(_ => Global))}
              />
              {tag == ""
                 ? React.null : <TabItem text={"#" ++ tag} active=true />}
            </ul>
          </div>
          <ArticleList listType={listType(tab)} />
        </div>
        <div className="col-md-3">
          <div className="sidebar">
            <TagsPopular onTagClick={tag => setTab(_ => Tag(tag))} />
          </div>
        </div>
      </div>
    </div>
  </div>;
};