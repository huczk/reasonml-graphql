[@react.component]
let make = () => {
  let route = Routes.useRoute();

  <>
    <Header currentRoute=route />
    {switch (route) {
     | Login => <Login />
     | Register => <Register />
     | Settings => <Settings />
     | CreateArticle => <Editor slug=None />
     | EditArticle(slug) => <Editor slug={Some(slug)} />
     | Article(slug) => <Article slug />
     | Profile(username, favorites) => <Profile username favorites />
     | Home => <Home />
     }}
    <Footer />
  </>;
};