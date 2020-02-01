let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

let authLink =
  ApolloLinks.createContextLink(() => {
    let token = Effects.getTokenFromStorage();

    {
      "headers": {
        "authorization": {j|Bearer $token|j},
      },
    };
  });

let httpLink =
  ApolloLinks.createHttpLink(
    ~uri="https://realworld-ts-gql.herokuapp.com/graphql",
    (),
  );

let client =
  ReasonApollo.createApolloClient(
    ~link=ApolloLinks.from([|authLink, httpLink|]),
    ~cache=inMemoryCache,
    (),
  );